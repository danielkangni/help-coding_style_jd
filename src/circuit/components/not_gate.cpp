#include "circuit/components/not_gate.hpp"
#include "error/nts_error.hpp"

namespace nts {

NotGate::NotGate(const std::string &name) : _name(name) {}

bool NotGate::is_output_pin(std::size_t pin)
{
    return pin == 2 || pin == 4 || pin == 6
        || pin == 8 || pin == 10 || pin == 12;
}

bool NotGate::is_input_pin(std::size_t pin)
{
    return pin == 1 || pin == 3 || pin == 5
        || pin == 9 || pin == 11 || pin == 13;
}

bool NotGate::is_power_pin(std::size_t pin)
{
    return pin == 7 || pin == 14;
}

Tristate NotGate::compute_inverter(std::size_t output_pin)
{
    std::size_t input_pin = (output_pin <= 6) ? output_pin - 1 : output_pin + 1;
    Tristate in = Tristate::Undefined;

    if (_links.find(input_pin) != _links.end()) {
        auto [comp, other_pin] = _links[input_pin];
        in = comp->compute_safe(other_pin);
    }
    return not_op(in);
}

Tristate NotGate::compute(std::size_t pin)
{
    if (is_output_pin(pin))
        return compute_inverter(pin);
    if (is_input_pin(pin))
        throw NtsError("Cannot compute input pin " + std::to_string(pin));
    if (is_power_pin(pin))
        throw NtsError("Pin " + std::to_string(pin) + " is a power pin");
    throw NtsError("Invalid pin number: " + std::to_string(pin));
}

void NotGate::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
{
    if (pin == 7 || pin == 14)
        throw NtsError("Cannot link power pin " + std::to_string(pin));
    if (pin < 1 || pin > 14)
        throw NtsError("Invalid pin number: " + std::to_string(pin));
    _links[pin] = {&other, other_pin};
}

}
