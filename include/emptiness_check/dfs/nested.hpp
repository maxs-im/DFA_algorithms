#pragma once

#include "automates/buchi.hpp"

namespace emptiness_check::dfs::nested
{

/// \brief Look for the accepting lasso in NBA automaton
/// The nested-DFS algorithm runs in time O(n+m) by using the first phase not only to discover the reachable accepting
/// states, but also to sort them. The searches of the second phase are conducted according to the order determined
/// by the sorting.
/// \note The algorithm, however, also has two important weak points:
///     It cannot be extended to NGAs, and it is not optimal
/// \param automat: investigated automaton
/// \return false if it finds at least one (first) lasso
bool is_empty(const automates::buchi &automat) noexcept;

} // namespace emptiness_check::dfs::nested
