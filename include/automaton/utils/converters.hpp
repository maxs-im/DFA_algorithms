#pragma once

#include "automates/buchi.hpp"

namespace utils::converters
{

/// \brief Conversion operation for NGA to NBA automatons
/// \note Will create a copy in case of NBA input
/// \param automat: NGA automaton
/// \return NBA automaton
automates::buchi nga2nba(const automates::buchi& automat) noexcept;

} // namespace utils::converters
