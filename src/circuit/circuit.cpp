#include "circuit/circuit.hpp"
#include "error/nts_error.hpp"
#include "circuit/components/input_component.hpp"
#include "circuit/components/output_component.hpp"
#include "circuit/components/clock_component.hpp"
#include "circuit/components/true_component.hpp"
#include "circuit/components/false_component.hpp"
#include "circuit/components/elementary_gates.hpp"
#include "circuit/components/quad_gates.hpp"
#include "circuit/components/not_gate.hpp"
#include "circuit/components/4008_adder.hpp"
#include "circuit/components/4512_selector.hpp"
#include <iostream>
#include <algorithm>

namespace nts {

Circuit::Circuit(const std::vector<ChipsetInfo> &chipsets,
                 const std::vector<LinkInfo> &links)
{
    create_components(chipsets);
    create_links(links);
}

using Creator = std::function<std::unique_ptr<IComponent>(const std::string &)>;

template<typename T>
static std::unique_ptr<IComponent> make(const std::string &name)
{
    return std::make_unique<T>(name);
}

static const std::unordered_map<std::string, Creator> s_factory = {
    {"input",  make<InputComponent>},
    {"output", make<OutputComponent>},
    {"clock",  make<ClockComponent>},
    {"true",   make<TrueComponent>},
    {"false",  make<FalseComponent>},
    {"not",    make<ElementaryNot>},
    {"and",    [](const std::string &n) { return std::make_unique<ElementaryGate>(n, and_op); }},
    {"or",     [](const std::string &n) { return std::make_unique<ElementaryGate>(n, or_op); }},
    {"xor",    [](const std::string &n) { return std::make_unique<ElementaryGate>(n, xor_op); }},
    {"4069",   make<NotGate>},
    {"4081",   make<AndGate>},
    {"4071",   make<OrGate>},
    {"4030",   make<XorGate>},
    {"4011",   make<NandGate>},
    {"4001",   make<NorGate>},
    {"4008",   make<Adder4008>},
    {"4512",   make<Selector4512>},
};

void Circuit::create_components(const std::vector<ChipsetInfo> &chipsets)
{
    for (const ChipsetInfo &chip : chipsets) {
        auto it = s_factory.find(chip.type);
        if (it == s_factory.end())
            throw NtsError("Unknown component type: " + chip.type);

        _components[chip.name] = it->second(chip.name);

        if (chip.type == "input" || chip.type == "clock")
            _input_names.push_back(chip.name);
        else if (chip.type == "output")
            _output_names.push_back(chip.name);
    }
    std::sort(_input_names.begin(), _input_names.end());
    std::sort(_output_names.begin(), _output_names.end());
}

void Circuit::create_links(const std::vector<LinkInfo> &links)
{
    for (const LinkInfo &link : links) {
        IComponent *comp1 = get_component(link.component1);
        IComponent *comp2 = get_component(link.component2);

        if (!comp1)
            throw NtsError("Unknown component: " + link.component1);
        if (!comp2)
            throw NtsError("Unknown component: " + link.component2);
        comp1->set_link(link.pin1, *comp2, link.pin2);
        comp2->set_link(link.pin2, *comp1, link.pin1);
    }
}

IComponent *Circuit::get_component(const std::string &name)
{
    auto it = _components.find(name);
    if (it == _components.end())
        return nullptr;
    return it->second.get();
}

void Circuit::set_input(const std::string &name, Tristate value)
{
    IComponent *comp = get_component(name);
    if (!comp)
        throw NtsError("Unknown component: " + name);

    if (auto input = dynamic_cast<InputComponent*>(comp)) {
        input->set_value(value);
        return;
    }
    if (auto clock = dynamic_cast<ClockComponent*>(comp)) {
        clock->set_value(value);
        return;
    }
    throw NtsError("Component " + name + " is not an input or clock");
}

void Circuit::simulate()
{
    for (auto &[name, comp] : _components) {
        comp->simulate();
    }
    _tick++;
}

void Circuit::display_pin(const std::string &name, Tristate value) const
{
    std::cout << "  " << name << ": ";
    if (value == Tristate::True)
        std::cout << "1";
    else if (value == Tristate::False)
        std::cout << "0";
    else
        std::cout << "U";
    std::cout << std::endl;
}


void Circuit::display() const
{
    std::cout << "tick: " << _tick << std::endl;

    std::cout << "input(s):" << std::endl;
    for (const std::string &name : _input_names) {
        auto it = _components.find(name);
        if (it != _components.end()) {
            Tristate value = Tristate::Undefined;

            if (auto input = dynamic_cast<InputComponent*>(it->second.get()))
                value = input->get_value();
            else if (auto clock = dynamic_cast<ClockComponent*>(it->second.get()))
                value = clock->get_value();
            display_pin(name, value);
        }
    }

    std::cout << "output(s):" << std::endl;
    for (const std::string &name : _output_names) {
        auto it = _components.find(name);
        if (it != _components.end()) {
            if (auto output = dynamic_cast<OutputComponent*>(it->second.get()))
                display_pin(name, output->get_value());
        }
    }
}

}
