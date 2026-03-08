#include <criterion/criterion.h>
#include "circuit/components/output_component.hpp"
#include "circuit/components/input_component.hpp"
#include "circuit/components/true_component.hpp"
#include "circuit/components/false_component.hpp"
#include "error/nts_error.hpp"
#include <string>

using namespace nts;


Test(OutputComponent, default_get_value_is_undefined)
{
    OutputComponent c("out");
    cr_assert_eq(c.get_value(), Tristate::Undefined);
}

Test(OutputComponent, get_name_returns_name)
{
    OutputComponent c("my_output");
    cr_assert_str_eq(c.get_name().c_str(), "my_output");
}

Test(OutputComponent, compute_pin1_no_link_is_undefined)
{
    OutputComponent c("out");
    cr_assert_eq(c.compute(1), Tristate::Undefined);
}

Test(OutputComponent, compute_invalid_pin_throws)
{
    OutputComponent c("out");
    cr_assert_throw(c.compute(2), NtsError);
}

Test(OutputComponent, compute_pin0_throws)
{
    OutputComponent c("out");
    cr_assert_throw(c.compute(0), NtsError);
}

Test(OutputComponent, compute_and_get_value_consistent)
{
    OutputComponent c("out");
    TrueComponent src("t");
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.compute(1), c.get_value());
}

Test(OutputComponent, linked_to_true_returns_true)
{
    OutputComponent c("out");
    TrueComponent src("t");
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.get_value(), Tristate::True);
}

Test(OutputComponent, linked_to_false_returns_false)
{
    OutputComponent c("out");
    FalseComponent src("f");
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(OutputComponent, linked_to_input_undefined_returns_undefined)
{
    OutputComponent c("out");
    InputComponent src("in");
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.get_value(), Tristate::Undefined);
}

Test(OutputComponent, linked_to_input_true_after_simulate)
{
    OutputComponent c("out");
    InputComponent src("in");
    src.set_value(Tristate::True);
    src.simulate();
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.get_value(), Tristate::True);
}

Test(OutputComponent, linked_to_input_false_after_simulate)
{
    OutputComponent c("out");
    InputComponent src("in");
    src.set_value(Tristate::False);
    src.simulate();
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(OutputComponent, reflects_input_value_change)
{
    OutputComponent c("out");
    InputComponent src("in");
    c.set_link(1, src, 1);
    src.set_link(1, c, 1);
    src.set_value(Tristate::True);
    src.simulate();
    cr_assert_eq(c.get_value(), Tristate::True);
    src.set_value(Tristate::False);
    src.simulate();
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(OutputComponent, set_link_valid_pin)
{
    OutputComponent c("out");
    TrueComponent src("t");
    cr_assert_no_throw(c.set_link(1, src, 1));
}

Test(OutputComponent, set_link_invalid_pin_throws)
{
    OutputComponent c("out");
    TrueComponent src("t");
    cr_assert_throw(c.set_link(2, src, 1), NtsError);
}

Test(OutputComponent, set_link_pin0_throws)
{
    OutputComponent c("out");
    TrueComponent src("t");
    cr_assert_throw(c.set_link(0, src, 1), NtsError);
}

Test(OutputComponent, overwriting_link_uses_new_source)
{
    OutputComponent c("out");
    TrueComponent t("t");
    FalseComponent f("f");
    c.set_link(1, t, 1);
    cr_assert_eq(c.get_value(), Tristate::True);
    c.set_link(1, f, 1);
    cr_assert_eq(c.get_value(), Tristate::False);
}

Test(OutputComponent, simulate_does_nothing)
{
    OutputComponent c("out");
    TrueComponent src("t");
    c.set_link(1, src, 1);
    c.simulate();
    cr_assert_eq(c.get_value(), Tristate::True);
}

Test(OutputComponent, compute_safe_breaks_cycle)
{
    OutputComponent c("out");
    c.set_link(1, c, 1);
    cr_assert_eq(c.compute(1), Tristate::Undefined);
}
