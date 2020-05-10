#pragma once

#include "finite_automaton.hpp"

namespace automates
{

/// \class Nondeterministic finite automaton
class nfa : public finite_automaton
{
public:

    /// \brief Creates an dfa objects with simple input verification (by asserting)
    /// \param states: a number of states.
    /// \param start: a correct initial/start state
    /// \param finals: a correct set of final states
    /// \param trans_table: a correct transition table
    /// \note  @m_alphabet sets by dividing transition table size into number of states
    explicit nfa(uint32_t states, uint32_t start, std::vector<uint32_t> finals, std::vector<uint32_t> trans_table);

    /// \brief Prints Q x Q table with elements means transaction symbol:
    ///     "->" - at the start of the raw means initial state
    ///     "*" - at the start of the raw means final state
    /// \note not displayed same initial|final info for each column
    /// \param out: output stream
    virtual void print(std::ostream &out) noexcept override;
};

/// \brief Construct nondeterministic finite automate from input stream
///     first num - a number of states. That means a set of states started from 0 to num-1
///     second num - an initial/start state: [0, first num)
///     third num - a number of final states [0, first num)
///     fifth vector - a vector o final states (size according to the third parameter)
///     sixth vector/matrix - a transition table for input alphabet.
///         With size equal to 'fourth num'^2 and values from alphabet
/// \note alphabet will be meant a number of the unique elements in the transition table
/// \param in: input stream
/// \return constructed automaton
nfa construct_read(std::istream &in);

} // namespace automates
