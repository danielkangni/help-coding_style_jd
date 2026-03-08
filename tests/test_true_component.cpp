#include <criterion/criterion.h>
#include "circuit/components/true_component.hpp"
#include "error/nts_error.hpp"

using namespace nts;

Test(TrueComponent, compute_pin1_is_true)
{
    TrueComponent c("t");
    cr_assert_eq(c.compute(1), Tristate::True);
}

Test(TrueComponent, compute_invalid_pin_throws)
{
    TrueComponent c("t");
    cr_assert_throw(c.compute(2), NtsError);
}

Test(TrueComponent, set_link_valid_pin)
{
    TrueComponent c("t");
    TrueComponent other("other");
    cr_assert_no_throw(c.set_link(1, other, 1));
}

Test(TrueComponent, set_link_invalid_pin_throws)
{
    TrueComponent c("t");
    TrueComponent other("other");
    cr_assert_throw(c.set_link(2, other, 1), NtsError);
}

Test(TrueComponent, simulate_does_not_change_value)
{
    TrueComponent c("t");
    c.simulate();
    cr_assert_eq(c.compute(1), Tristate::True);
}

Test(TrueComponent, always_returns_true)
{
    TrueComponent c("t");
    for (int i = 0; i < 5; i++) {
        c.simulate();
        cr_assert_eq(c.compute(1), Tristate::True);
    }
}

Test(TrueComponent, get_name_returns_name)
{
    TrueComponent c("my_true");
    cr_assert_str_eq(c.get_name().c_str(), "my_true");
}
