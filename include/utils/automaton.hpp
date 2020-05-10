#pragma once

#include <vector>
#include <iostream>

/// \brief deterministic finite automaton
namespace dfa
{

/// \class automaton - handle dfa data with its common simple actions
class automaton
{
public:
    automaton() = delete;
    automaton& operator=(const automaton&) = delete;

    /// \brief Creates an dfa objects with simple input verification (by asserting)
    /// \param _states: a number of states.
    /// \param _start: a correct initial/start state
    /// \param _final_states: a correct set of final states
    /// \param _trans_table: a correct transition table
    explicit automaton(
        uint32_t _states, uint32_t _start,
        std::vector<uint32_t> _final_states,
        std::vector<uint32_t> _trans_table
    );

    /// \brief Calculates automaton state by its transition function
    /// \param curr: current state
    /// \param sym: symbol from alphabet
    /// \return state in which the automaton will be
    uint32_t action(uint32_t curr, uint32_t sym) const noexcept;

    /// \brief Get a number of unique symbols in alphabet by dividing transition table size into number of states
    /// \return size of an alphabet set
    uint32_t get_alphabet_num() const noexcept;

    /// \brief Getter for the initial/start state of the automaton
    /// \return initial state q0
    uint32_t get_initial_state() const noexcept { return start; }

    /// \brief Check if input number is an accept state
    /// \param st: state of automaton
    /// \return whether state belongs to @final_states
    bool is_final(uint32_t st) const noexcept;

    /// \brief Simple and user-friendly automaton representation
    /// \param out: output stream
    void print(std::ostream &out);

private:

    /// \brief a finite set of states Q [0, @states)
    const uint32_t states;
    /// \brief an initial or start state q0 ∈ Q
    const uint32_t start;
    /// \brief a set of accept states F ⊆ Q
    const std::vector<uint32_t> final_states;
    /// \brief a transition function δ : Q x ∑ (alphabet) → Q
    const std::vector<uint32_t> trans_table;

};

/// \brief Construct deterministic finite automaton from input stream
///     first num - a number of states. That means a set of states started from 0 to num-1
///     second num - an initial/start state: [0, first num)
///     third num - a number of final states [0, first num)
///     fourth num - a number of input symbols called the alphabet. That means an alphabet set started from 0 to num-1
///     fifth vector - a vector o final states (size according to the third parameter)
///     sixth vector/matrix - a transition table for input alphabet
///         With size equal to fourth num X first num
/// \param in: input stream
/// \return constructed automaton
automaton construct_read(std::istream &in);

} // namespace dfa
