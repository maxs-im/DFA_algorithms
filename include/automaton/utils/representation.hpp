#pragma once

#include "automates/buchi.hpp"

#include <istream>

namespace utils::representation
{

/// \brief Construct nondeterministic Büchi automaton from input stream
///     State definition:
///         first num - a number of states > 0. That means a set of states [0, num)
///         second num - an initial/start state: [0, first num)
///     Final states definition:
///         third num - a number of final states sets [0, num]
///         Cycle:
///             fourth num - a number of final states in current set. That means an alphabet: [0, first num]
///             next "fourth num" nums - final states for current set
///     Transition table read (specified by @paired):
///         Not-paired (@paired = false):
///             vector/matrix - a transition table that constructs from each state (row) for each state (column)
///                             (right->down decreasing in range [0, first num)) with symbols from alphabet in the cells
///             Note: Size equal to 'fourth num' x 'first num' and values from alphabet.
///                   Use "0" (EMPTY) symbol to ignore the edge
///         Paired (@paired = true):
///             Cycle while EOF:
///                 state num - number of the start state
///                 symbol num - symbol that accepts state num
///                 successor num - state after acceptancy
/// \param in: input stream
/// \param paired: specify transition reader method. Used "state -> symbol -> state" by default
/// \return constructed automaton
automates::buchi::nba construct_read(std::istream &in, bool paired = true) noexcept;

}
