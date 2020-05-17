#pragma once

#include "automates/buchi.hpp"

#include <istream>
#include <memory>

namespace utils::representation
{

/// \brief Construct nondeterministic Büchi automaton from input stream
///     Final states definition:
///         set num - a number of final states sets [0, num]
///         Cycle:
///             set_in num - a number of final states in current set.
///             next "set_in num" nums - final states for current set
///     Transition table read (specified by @paired):
///         Cycle while EOF:
///             state num - number of the start state
///             symbol num - symbol that accepts state num
///             successor num - state after acceptancy
/// \note: Hints will be printed to the std::cout in case of in == std::cin
/// \param in: input stream
/// \return constructed automaton
automates::buchi construct_read(std::istream &in) noexcept;

}
