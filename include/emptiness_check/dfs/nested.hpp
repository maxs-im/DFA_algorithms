#pragma once

#include "automates/buchi.hpp"

namespace emptiness_check::dfs::nested
{

bool is_empty(const automates::buchi &automat);

} // namespace emptiness_check::dfs::nested
