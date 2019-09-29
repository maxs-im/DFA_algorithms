#pragma once

#include <automaton.hpp>

namespace binary_algo
{
    enum OPERATOR
    { ASSOCIATION, INTERSECTION, DIFFERENCE };

    dfa::automaton do_act(
        const dfa::automaton& a, const dfa::automaton& b, 
        OPERATOR op
    );

    namespace
    {
        bool act_condition(bool a, bool b, OPERATOR op);
    }
}