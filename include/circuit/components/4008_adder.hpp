#pragma once
#include "circuit/i_component.hpp"
#include "circuit/components/logic_ops.hpp"
#include <string>
#include <map>
#include <functional>

namespace nts {
class Adder4008 : public IComponent {
public:
    Adder4008(const std::string &name);

    Tristate compute(std::size_t pin) override;

    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;

    void simulate() override {}

    const std::string &get_name() const override { return _name; }
private:
    std::string _name;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};
}