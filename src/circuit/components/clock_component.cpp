#include "circuit/components/clock_component.hpp"
#include "error/nts_error.hpp"

namespace nts {

Tristate ClockComponent::compute(std::size_t pin)
{
    if (pin != 1)
        throw NtsError("Clock component only has pin 1");
    return _current;
}

void ClockComponent::set_link(std::size_t pin,
    IComponent &other, std::size_t other_pin)
{
    if (pin != 1)
        throw NtsError("Clock component only has pin 1");
    _links[pin] = {&other, other_pin};
}

void ClockComponent::simulate()
{
    if (_has_pending) {
        _current = _pending;
        _has_pending = false;
    } else if (_current == Tristate::True)
        _current = Tristate::False;
    else if (_current == Tristate::False)
        _current = Tristate::True;
}

}
