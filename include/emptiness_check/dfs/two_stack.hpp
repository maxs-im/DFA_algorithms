#pragma once

#include "automates/buchi.hpp"

/// \brief The two-stack algorithm
namespace emptiness_check::dfs::two_stack
{

/// \brief Look for the accepting lasso in NBA/NGA automaton
/// \details While the state is grey, the algorithm tries to find a cycle containing it. If it succeeds, then the state
/// is removed from C. If not, then the state is removed from C when it is blackened. At any time t, the candidates are
/// the currently grey states that do not belong to any cycle of At.
/// \param automat: investigated automaton
/// \return false if it finds at least one (first) lasso
bool is_empty(const automates::buchi &automat) noexcept;

} // namespace emptiness_check::dfs::two_stack
