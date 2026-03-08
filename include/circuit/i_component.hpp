#pragma once

#include "circuit/tristate.hpp"
#include <cstddef>
#include <string>
#include <set>

namespace nts {

class IComponent {

    public:
        virtual ~IComponent() = default;

        virtual Tristate compute(std::size_t pin) = 0;

        Tristate compute_safe(std::size_t pin) {
            if (_computing_pins.count(pin))
                return Tristate::Undefined;
            _computing_pins.insert(pin);
            Tristate result = compute(pin);
            _computing_pins.erase(pin);
            return result;
        }

        virtual void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) = 0;

        virtual void simulate() = 0;

        virtual const std::string &get_name() const = 0;

    private:
        std::set<std::size_t> _computing_pins;
};

}
