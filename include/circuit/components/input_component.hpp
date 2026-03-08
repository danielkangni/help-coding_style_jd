#pragma once

#include "circuit/components/a_component.hpp"

namespace nts {

class InputComponent : public AComponent {
public:
    InputComponent(const std::string &name)
        : AComponent(name), _current(Tristate::Undefined),
          _pending(Tristate::Undefined), _has_pending(false) {}

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    void simulate() override;
    void set_value(Tristate value) {
        _pending = value;
        _has_pending = true;
    }
    Tristate get_value() const {
        return _current;
    }

private:
    Tristate _current;
    Tristate _pending;
    bool _has_pending;
};

}
