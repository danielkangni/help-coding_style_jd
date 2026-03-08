#include "circuit/components/4008_adder.hpp"
#include "error/nts_error.hpp"

namespace nts {
constexpr std::size_t PIN_A[4] = {7, 5, 3, 1};
constexpr std::size_t PIN_B[4] = {6, 4, 2, 15};
constexpr std::size_t PIN_CARRY_IN = 9;
constexpr std::size_t PIN_SUM[4] = {10, 11, 12, 13};
constexpr std::size_t PIN_CARRY_OUT = 14;

Adder4008::Adder4008(const std::string &name) : _name(name)
{
}

Tristate get_input(const std::map<std::size_t, std::pair<IComponent*, std::size_t>> &links, std::size_t pin)
{
    if (links.find(pin) != links.end()) {
        auto [comp, otherPin] = links.at(pin);
        return comp->compute_safe(otherPin);
    }
    return Tristate::Undefined;
}

Tristate Adder4008::compute(std::size_t pin)
{
    Tristate a[4];
    Tristate b[4];
    if (pin < 1 || pin > 16)
      throw NtsError("Invalid pin number: " + std::to_string(pin));
    for (int i = 0; i < 4; i++) {
        a[i] = get_input(_links, PIN_A[i]);
        b[i] = get_input(_links, PIN_B[i]);
    }
    Tristate carry_in = get_input(_links, PIN_CARRY_IN);
    Tristate carry = carry_in;
    for (int i = 0; i < 4; i++) {
        if (pin == PIN_SUM[i]) {
            Tristate sum = xor_op(xor_op(a[i], b[i]), carry);
            return sum;
        }
        carry = or_op(and_op(a[i], b[i]), and_op(carry, or_op(a[i], b[i])));
    }
    if (pin == PIN_CARRY_OUT)
        return carry;
    return Tristate::Undefined;
}

void Adder4008::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
{
    if (pin < 1 || pin > 16)
       throw NtsError("Invalid pin number: " + std::to_string(pin));
    _links[pin] = {&other, other_pin};
}

}