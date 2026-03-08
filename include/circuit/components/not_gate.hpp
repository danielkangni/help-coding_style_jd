#pragma once

#include "circuit/i_component.hpp"
#include "circuit/components/logic_ops.hpp"
#include <string>
#include <map>

namespace nts {

class NotGate : public IComponent {
public:
    NotGate(const std::string &name);

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    void simulate() override {}
    const std::string &get_name() const override { return _name; }

private:
    static bool is_output_pin(std::size_t pin);
    static bool is_input_pin(std::size_t pin);
    static bool is_power_pin(std::size_t pin);
    Tristate compute_inverter(std::size_t output_pin);

    std::string _name;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};

}
