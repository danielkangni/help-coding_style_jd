#include "circuit/components/elementary_gates.hpp"
#include "error/nts_error.hpp"

namespace nts {

ElementaryGate::ElementaryGate(const std::string &name,
    std::function<Tristate(Tristate, Tristate)> operation)
    : _name(name), _operation(operation)
{
}

Tristate ElementaryGate::compute(std::size_t pin)
{
    if (pin == 3) {
        Tristate in1 = Tristate::Undefined;
        Tristate in2 = Tristate::Undefined;

        if (_links.find(1) != _links.end()) {
            auto [comp, other_pin] = _links[1];
            in1 = comp->compute_safe(other_pin);
        }
        if (_links.find(2) != _links.end()) {
            auto [comp, other_pin] = _links[2];
            in2 = comp->compute_safe(other_pin);
        }
        return _operation(in1, in2);
    }
    if (pin == 1 || pin == 2)
        throw NtsError("Cannot compute input pin " + std::to_string(pin));
    throw NtsError("Invalid pin number: " + std::to_string(pin));
}

void ElementaryGate::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
{
    if (pin < 1 || pin > 3)
        throw NtsError("Invalid pin number: " + std::to_string(pin));
    _links[pin] = {&other, other_pin};
}

ElementaryNot::ElementaryNot(const std::string &name) : _name(name) {}

Tristate ElementaryNot::compute(std::size_t pin)
{
    if (pin == 2) {
        Tristate in = Tristate::Undefined;
        if (_links.find(1) != _links.end()) {
            auto [comp, other_pin] = _links[1];
            in = comp->compute_safe(other_pin);
        }
        return not_op(in);
    }
    if (pin == 1)
        throw NtsError("Cannot compute input pin 1");
    throw NtsError("Invalid pin number: " + std::to_string(pin));
}

void ElementaryNot::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
{
    if (pin < 1 || pin > 2)
        throw NtsError("Invalid pin number: " + std::to_string(pin));
    _links[pin] = {&other, other_pin};
}

}