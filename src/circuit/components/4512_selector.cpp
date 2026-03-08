#include "circuit/components/4512_selector.hpp"
#include "error/nts_error.hpp"

namespace nts {
    constexpr std::size_t A = 11;
    constexpr std::size_t B = 12;
    constexpr std::size_t C = 13;
    constexpr std::size_t oe = 15;
    constexpr std::size_t Inhibit = 10;
    Selector4512::Selector4512(const std::string &name) : _name(name)
    {
    }

    Tristate get_input_b(const std::map<std::size_t, std::pair<IComponent*, std::size_t>> &links, std::size_t pin)
    {
        if (links.find(pin) != links.end()) {
            auto [comp, otherPin] = links.at(pin);
            return comp->compute_safe(otherPin);
        }
        return Tristate::Undefined;
    }

    Tristate Selector4512::compute(std::size_t pin)
    {
        if (pin < 1 || pin > 16)
            throw NtsError("Invalid pin number: " + std::to_string(pin));
        if (get_input_b(_links, oe) != Tristate::False) {
            return Tristate::Undefined;
        }
        if (get_input_b(_links, Inhibit) == Tristate::True) {
            return Tristate::False;
        }
        Tristate a = get_input_b(_links, A);
        Tristate b = get_input_b(_links, B);
        Tristate c = get_input_b(_links, C);
        if (a == Tristate::Undefined ||
            b == Tristate::Undefined ||
            c == Tristate::Undefined) {
            return Tristate::Undefined;
        }
        std::size_t index = 0;
        if (a == Tristate::True)
            index += 1;
        if (b == Tristate::True)
            index += 2;
        if (c == Tristate::True)
            index += 4;
        static const std::size_t input[8] = {1, 2, 3, 4, 5, 6, 7, 9};
        return get_input_b(_links, input[index]);
    }

    void Selector4512::set_link(std::size_t pin, IComponent &other, std::size_t other_pin)
    {   if (pin < 1 || pin > 16)
            throw NtsError("Invalid pin number: " + std::to_string(pin));
        _links[pin] = {&other, other_pin};
    }
}