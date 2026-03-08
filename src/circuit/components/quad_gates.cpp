#include "circuit/components/quad_gates.hpp"
#include "error/nts_error.hpp"

namespace nts {

QuadGate::QuadGate(const std::string &name,
                   std::function<Tristate(Tristate, Tristate)> operation)
    : _name(name), _operation(operation)
{
}

Tristate QuadGate::compute_gate(std::size_t in1_pin, std::size_t in2_pin)
{
    Tristate in1 = Tristate::Undefined;
    Tristate in2 = Tristate::Undefined;

    if (_links.find(in1_pin) != _links.end()) {
        auto [comp, other_pin] = _links[in1_pin];
        in1 = comp->compute_safe(other_pin);
    }
    if (_links.find(in2_pin) != _links.end()) {
        auto [comp, other_pin] = _links[in2_pin];
        in2 = comp->compute_safe(other_pin);
    }
    return _operation(in1, in2);
}

Tristate QuadGate::compute(std::size_t pin)
{
    switch (pin) {
        case 3:  return compute_gate(1, 2);
        case 4:  return compute_gate(5, 6);
        case 10: return compute_gate(8, 9);
        case 11: return compute_gate(12, 13);
        case 1: case 2: case 5: case 6:
        case 8: case 9: case 12: case 13:
            throw NtsError("Cannot compute input pin " + std::to_string(pin));
        case 7: case 14:
            throw NtsError("Pin " + std::to_string(pin) + " is a power pin");
        default:
            throw NtsError("Invalid pin number: " + std::to_string(pin));
    }
}

void QuadGate::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
{
    if (pin == 7 || pin == 14)
        throw NtsError("Cannot link power pin " + std::to_string(pin));
    if (pin < 1 || pin > 14)
        throw NtsError("Invalid pin number: " + std::to_string(pin));
    _links[pin] = {&other, other_pin};
}

AndGate::AndGate(const std::string &name) : QuadGate(name, and_op) {}
OrGate::OrGate(const std::string &name) : QuadGate(name, or_op) {}
XorGate::XorGate(const std::string &name) : QuadGate(name, xor_op) {}
NandGate::NandGate(const std::string &name) : QuadGate(name, nand_op) {}
NorGate::NorGate(const std::string &name) : QuadGate(name, nor_op) {}

}
