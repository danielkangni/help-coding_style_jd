#include "circuit/components/true_component.hpp"
#include "error/nts_error.hpp"

namespace nts {

Tristate TrueComponent::compute(std::size_t pin)
{
    if (pin != 1)
        throw NtsError("True component only has pin 1");
    return Tristate::True;
}

void TrueComponent::set_link(std::size_t pin,
    IComponent &other, std::size_t other_pin)
{
    if (pin != 1)
        throw NtsError("True component only has pin 1");
    _links[pin] = {&other, other_pin};
}

}
