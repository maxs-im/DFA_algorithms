#pragma once

#include "automates/buchi.hpp"

namespace emptiness_check::dfs::two_stack
{

bool is_empty(const automates::buchi::nba &automat);

} // namespace emptiness_check::dfs::two_stack
