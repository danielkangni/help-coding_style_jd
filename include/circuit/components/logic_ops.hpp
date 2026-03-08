#pragma once

#include "circuit/tristate.hpp"

namespace nts {

inline Tristate and_op(Tristate a, Tristate b)
{
    if (a == Tristate::False || b == Tristate::False)
        return Tristate::False;
    if (a == Tristate::True && b == Tristate::True)
        return Tristate::True;
    return Tristate::Undefined;
}

inline Tristate or_op(Tristate a, Tristate b)
{
    if (a == Tristate::True || b == Tristate::True)
        return Tristate::True;
    if (a == Tristate::False && b == Tristate::False)
        return Tristate::False;
    return Tristate::Undefined;
}

inline Tristate xor_op(Tristate a, Tristate b)
{
    if (a == Tristate::Undefined || b == Tristate::Undefined)
        return Tristate::Undefined;
    return (a != b) ? Tristate::True : Tristate::False;
}

inline Tristate nand_op(Tristate a, Tristate b)
{
    if (a == Tristate::False || b == Tristate::False)
        return Tristate::True;
    if (a == Tristate::True && b == Tristate::True)
        return Tristate::False;
    return Tristate::Undefined;
}

inline Tristate nor_op(Tristate a, Tristate b)
{
    if (a == Tristate::True || b == Tristate::True)
        return Tristate::False;
    if (a == Tristate::False && b == Tristate::False)
        return Tristate::True;
    return Tristate::Undefined;
}

inline Tristate not_op(Tristate a)
{
    if (a == Tristate::True)
        return Tristate::False;
    if (a == Tristate::False)
        return Tristate::True;
    return Tristate::Undefined;
}

}
