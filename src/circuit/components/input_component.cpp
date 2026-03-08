#include "circuit/components/input_component.hpp"
#include "error/nts_error.hpp"

namespace nts {

Tristate InputComponent::compute(std::size_t pin)
{
    if (pin != 1)
        throw NtsError("Input component only has pin 1");
    return _current;
}

void InputComponent::simulate()
{
    if (_has_pending) {
        _current = _pending;
        _has_pending = false;
    }
}

void InputComponent::set_link(std::size_t pin,
    IComponent &other, std::size_t other_pin)
{
    if (pin != 1)
        throw NtsError("Input component only has pin 1");
    _links[pin] = {&other, other_pin};
}

}
