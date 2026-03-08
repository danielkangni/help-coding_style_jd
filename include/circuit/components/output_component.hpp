#pragma once

#include "circuit/components/a_component.hpp"

namespace nts {

class OutputComponent : public AComponent {
public:
    OutputComponent(const std::string &name) : AComponent(name) {}

    Tristate compute(std::size_t pin) override;
    void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
    Tristate get_value();

private:
    Tristate _cached_value = Tristate::Undefined;
};

}
