#pragma once

#include <automaton.hpp>

namespace binary_algo
{
    enum OPERATOR
    { ASSOCIATION, INTERSECTION, DIFFERENCE };

    dfa::automaton do_act(
        const dfa::automaton& a, const dfa::automaton& b, 
        OPERATOR op,
        bool pair = true
    );

    namespace
    {
        void print_pairs(
            const std::vector<std::pair<uint32_t, uint32_t>> &states,
            const std::pair<uint32_t, uint32_t>& start, 
            const std::vector<std::pair<uint32_t, uint32_t>> &final_states,
            const std::vector<std::pair<uint32_t, uint32_t>> &table
        );

        bool act_condition(bool a, bool b, OPERATOR op);
    }
}