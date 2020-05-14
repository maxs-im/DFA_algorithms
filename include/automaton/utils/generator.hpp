#pragma once

#include "automates/buchi.hpp"

namespace utils::generator
{

struct seed_automata
{
    uint32_t q_num = 0;
    uint32_t alph = 0;
    uint32_t final_num_sets = 0;
    uint32_t max_final_states = 0;
    uint32_t random_edges = 0;
};

automates::buchi generate_nba(seed_automata& seed);

} // namespace utils::generator
