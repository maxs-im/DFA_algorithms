#pragma once

#include <automates/dfa.hpp>

namespace binary_algo
{
    enum OPERATOR
    { ASSOCIATION, INTERSECTION, DIFFERENCE };

    automates::dfa do_act(
            const automates::dfa& a, const automates::dfa& b,
            OPERATOR op,
            bool pair
    );

    namespace
    {
        void minimizate(
            std::vector<std::pair<uint32_t, uint32_t>> &states,
            std::vector<std::pair<uint32_t, uint32_t>> &table
        );

        void print_pairs(
            const std::vector<std::pair<uint32_t, uint32_t>> &states,
            const std::pair<uint32_t, uint32_t>& start, 
            const std::vector<std::pair<uint32_t, uint32_t>> &final_states,
            const std::vector<std::pair<uint32_t, uint32_t>> &table
        );

        bool act_condition(bool a, bool b, OPERATOR op);
    }
}