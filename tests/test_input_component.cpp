#include <criterion/criterion.h>
#include "circuit/components/input_component.hpp"
#include "error/nts_error.hpp"
#include <string>

using namespace nts;


Test(InputComponent, default_value_is_undefined)
{
    InputComponent c("in");
    cr_assert_eq(c.get_value(), Tristate::Undefined);
}

Test(InputComponent, get_name_returns_name)
{
    InputComponent c("my_input");
    cr_assert_str_eq(c.get_name().c_str(), "my_input");
}


Test(InputComponent, compute_pin1_returns_undefined_before_simulate)
{
    InputComponent c("in");
    cr_assert_eq(c.compute(1), Tristate::Undefined);
}

Test(InputComponent, compute_pin1_returns_true_after_simulate)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.simulate();
    cr_assert_eq(c.compute(1), Tristate::True);
}

Test(InputComponent, compute_pin1_returns_false_after_simulate)
{
    InputComponent c("in");
    c.set_value(Tristate::False);
    c.simulate();
    cr_assert_eq(c.compute(1), Tristate::False);
}

Test(InputComponent, compute_invalid_pin_throws)
{
    InputComponent c("in");
    cr_assert_throw(c.compute(0), NtsError);
}

Test(InputComponent, compute_pin2_throws)
{
    InputComponent c("in");
    cr_assert_throw(c.compute(2), NtsError);
}


Test(InputComponent, set_value_pending_not_visible_before_simulate)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    cr_assert_eq(c.get_value(), Tristate::Undefined);
}

Test(InputComponent, simulate_applies_true)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::True);
}

Test(InputComponent, simulate_applies_false)
{
    InputComponent c("in");
    c.set_value(Tristate::False);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(InputComponent, simulate_applies_undefined)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.simulate();
    c.set_value(Tristate::Undefined);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::Undefined);
}

Test(InputComponent, last_set_value_wins_before_simulate)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.set_value(Tristate::False);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(InputComponent, simulate_without_pending_keeps_value)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.simulate();
    c.simulate();
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::True);
}

Test(InputComponent, value_changes_across_ticks)
{
    InputComponent c("in");
    c.set_value(Tristate::True);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::True);
    c.set_value(Tristate::False);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::False);
}


Test(InputComponent, set_link_valid_pin)
{
    InputComponent c("in");
    InputComponent other("other");
    cr_assert_no_throw(c.set_link(1, other, 1));
}

Test(InputComponent, set_link_invalid_pin_throws)
{
    InputComponent c("in");
    InputComponent other("other");
    cr_assert_throw(c.set_link(2, other, 1), NtsError);
}

Test(InputComponent, set_link_pin0_throws)
{
    InputComponent c("in");
    InputComponent other("other");
    cr_assert_throw(c.set_link(0, other, 1), NtsError);
}


Test(InputComponent, error_what_message)
{
    InputComponent c("in");
    cr_assert_throw(c.compute(2), NtsError);
    try {
        c.compute(2);
    } catch (const NtsError &e) {
        cr_assert(std::string(e.what()).size() > 0);
    }
}
