#pragma once

#include "circuit/components/a_component.hpp"

namespace nts {

class TrueComponent : public AComponent {
public:
    TrueComponent(const std::string &name) : AComponent(name) {}

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
};

}
