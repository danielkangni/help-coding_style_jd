#pragma once

#include "circuit/i_component.hpp"
#include "circuit/components/logic_ops.hpp"
#include <string>
#include <map>
#include <functional>

namespace nts {

class QuadGate : public IComponent {
public:
    QuadGate(const std::string &name,
        std::function<Tristate(Tristate, Tristate)> operation);

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    void simulate() override {}
    const std::string &get_name() const override {
        return _name;
    }

private:
    Tristate compute_gate(std::size_t in1_pin, std::size_t in2_pin);

    std::string _name;
    std::function<Tristate(Tristate, Tristate)> _operation;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};

class AndGate : public QuadGate {
public:
    AndGate(const std::string &name);
};

class OrGate : public QuadGate {
public:
    OrGate(const std::string &name);
};

class XorGate : public QuadGate {
public:
    XorGate(const std::string &name);
};

class NandGate : public QuadGate {
public:
    NandGate(const std::string &name);
};

class NorGate : public QuadGate {
public:
    NorGate(const std::string &name);
};

}
