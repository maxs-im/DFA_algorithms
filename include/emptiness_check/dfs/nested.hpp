#pragma once

#include "automaton.hpp"

namespace emptiness_check::dfs::nested
{

bool is_empty(const dfa::automaton &automat);

} // namespace emptiness_check::dfs::nested
