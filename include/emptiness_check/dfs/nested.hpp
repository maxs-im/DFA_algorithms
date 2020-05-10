#pragma once

#include "automates/dfa.hpp"

namespace emptiness_check::dfs::nested
{

bool is_empty(const automates::dfa &automat);

} // namespace emptiness_check::dfs::nested
