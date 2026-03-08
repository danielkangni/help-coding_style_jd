#pragma once

#include "circuit/i_component.hpp"
#include "circuit/components/logic_ops.hpp"
#include <string>
#include <map>
#include <functional>

namespace nts {

class ElementaryGate : public IComponent {
public:
    ElementaryGate(const std::string &name,
        std::function<Tristate(Tristate, Tristate)> operation);

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    void simulate() override {}
    const std::string &get_name() const override { return _name; }

private:
    std::string _name;
    std::function<Tristate(Tristate, Tristate)> _operation;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};

class ElementaryNot : public IComponent {
public:
    ElementaryNot(const std::string &name);

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    void simulate() override {}
    const std::string &get_name() const override { return _name; }

private:
    std::string _name;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};

}
