#pragma once

#include "automates/buchi.hpp"

namespace utils::converters
{

/// \brief Conversion operation from NGA to NBA automaton
/// \param automat: NGA automaton
/// \return NBA automaton or nullopt if @automat is already NBA
std::optional<automates::buchi> nga2nba(const automates::buchi& automat) noexcept;

} // namespace utils::converters
