#pragma once

#include <vector>
#include <iostream>

#include "finite_automaton.hpp"

namespace automates
{

/// \class Deterministic finite automate
class dfa : public finite_automaton
{
public:

    /// \brief Creates an dfa objects with simple input verification (by asserting)
    /// \param states: a number of states.
    /// \param start: a correct initial/start state
    /// \param finals: a correct set of final states
    /// \param trans_table: a correct transition table
    /// \note  @m_alphabet sets by dividing transition table size into number of states
    explicit dfa(uint32_t states, uint32_t start, std::vector<uint32_t> finals, std::vector<uint32_t> trans_table);

    /// \brief Calculates dfa state by its transition function
    /// \param curr: current state
    /// \param sym: symbol from alphabet
    /// \return state in which the dfa will be
    uint32_t action(uint32_t curr, uint32_t sym) const noexcept;

    /// \brief Prints Q x ∑ table with elements means next state:
    ///     "->" - at the start of the raw means initial state
    ///     "*" - at the start of the raw means final state
    /// \param out: output stream
    virtual void print(std::ostream &out) noexcept override;

};

/// \brief Construct deterministic finite automate from input stream
///     first num - a number of states. That means a set of states started from 1 to num
///     second num - an initial/start state: (0, first num]
///     third num - a number of final states [0, first num]
///     fourth num - a number of input symbols called the alphabet. That means an alphabet: (0, num]
///     fifth vector - a vector o final states (size according to the 'third' parameter)
///     sixth vector/matrix - a transition table for input alphabet.
///         With size equal to 'fourth num' x 'first num' and values from states
/// \param in: input stream
/// \return constructed automaton
dfa construct_read(std::istream &in);

} // namespace automates
