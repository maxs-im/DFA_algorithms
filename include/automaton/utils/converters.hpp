#pragma once

#include "automates/buchi.hpp"

namespace utils::converters
{

/// \brief Conversion operation for NGA to NBA automatons
/// \param automat: NGA automaton
/// \return NBA automaton
automates::buchi::nba nga2nba(const automates::buchi::nba& automat) noexcept;

} // namespace utils::converters
