#include "circuit/components/output_component.hpp"
#include "error/nts_error.hpp"

namespace nts {

Tristate OutputComponent::compute(std::size_t pin)
{
    if (pin != 1)
        throw NtsError("Output component only has pin 1");

    if (_links.find(1) != _links.end()) {
        auto [comp, other_pin] = _links[1];
        return comp->compute_safe(other_pin);
    }
    return Tristate::Undefined;
}

void OutputComponent::set_link(std::size_t pin,
    IComponent &other, std::size_t other_pin)
{
    if (pin != 1)
        throw NtsError("Output component only has pin 1");
    _links[pin] = {&other, other_pin};
}

Tristate OutputComponent::get_value()
{
    return compute(1);
}

}
