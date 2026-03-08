#include <criterion/criterion.h>
#include "circuit/components/false_component.hpp"
#include "error/nts_error.hpp"

using namespace nts;

Test(FalseComponent, compute_pin1_is_false)
{
    FalseComponent c("f");
    cr_assert_eq(c.compute(1), Tristate::False);
}

Test(FalseComponent, compute_invalid_pin_throws)
{
    FalseComponent c("f");
    cr_assert_throw(c.compute(2), NtsError);
}

Test(FalseComponent, set_link_valid_pin)
{
    FalseComponent c("f");
    FalseComponent other("other");
    cr_assert_no_throw(c.set_link(1, other, 1));
}

Test(FalseComponent, set_link_invalid_pin_throws)
{
    FalseComponent c("f");
    FalseComponent other("other");
    cr_assert_throw(c.set_link(2, other, 1), NtsError);
}

Test(FalseComponent, simulate_does_not_change_value)
{
    FalseComponent c("f");
    c.simulate();
    cr_assert_eq(c.compute(1), Tristate::False);
}

Test(FalseComponent, always_returns_false)
{
    FalseComponent c("f");
    for (int i = 0; i < 5; i++) {
        c.simulate();
        cr_assert_eq(c.compute(1), Tristate::False);
    }
}

Test(FalseComponent, get_name_returns_name)
{
    FalseComponent c("my_false");
    cr_assert_str_eq(c.get_name().c_str(), "my_false");
}
