#pragma once

#include <vector>
#include <ostream>

namespace automates
{

/// \class Interface of the finite automaton A.
class finite_automaton
{
public:
    finite_automaton() = delete;
    finite_automaton& operator=(const finite_automaton&) = delete;

    /// \brief Creates an automaton object with simple input verification (by asserting)
    /// \param states: a number of states.
    /// \param alphabet: a number of symbols in alphabet
    /// \param start: a correct initial/start state
    /// \param finals: a correct set of final states
    /// \param table: a correct transition table
    explicit finite_automaton(const uint32_t states, const uint32_t alphabet, const uint32_t start,
                              std::vector<uint32_t> finals, std::vector<uint32_t> table)
        : m_states(states), m_alphabet(alphabet), m_start(start),
          m_final_states(std::move(finals)), m_trans_table(std::move(table))
    {
        assert(m_states && "Empty states");
        assert(m_start && m_start <= m_states && "Incorrect start/initial point");
        for (const auto &fs : m_final_states)
            assert(fs <= m_states && "Incorrect final state");
    }

    /// \brief Get a number of unique states
    /// \return size of an alphabet set
    virtual uint32_t get_states_num() const noexcept final { return m_states; }

    /// \brief Get a number of unique symbols in alphabet
    /// \return size of an alphabet set
    virtual uint32_t get_alphabet_num() const noexcept final { return m_alphabet; }

    /// \brief Getter for the initial/start state of the dfa
    /// \return initial state q0
    virtual uint32_t get_initial_state() const noexcept final { return m_start; }

    /// \brief Check if input number is an accept state
    /// \param st: state of dfa
    /// \return whether state belongs to @final_states
    bool is_final(uint32_t st) const noexcept
    {
        const auto &fs = m_final_states;
        return std::find(fs.begin(), fs.end(), st) != fs.end();
    }

    /// \brief Simple and user-friendly dfa representation
    /// \param out: output stream
    virtual void print(std::ostream &out) noexcept = 0;

protected:
    /// \note State = 0 does not exist. It can be used as empty condition

    /// \brief a finite set of states Q: (0, @states]
    const uint32_t m_states = 0;
    /// \brief a finite set called the alphabet of A: ∑
    const uint32_t m_alphabet = 0;
    /// \brief an initial or start state: q0 ∈ Q
    const uint32_t m_start = 0;

    /// \brief container for a set of accept states: F ⊆ Q
    /// \attention logic could be overridden
    const std::vector<uint32_t> m_final_states = {};
    /// \brief container for a transition function: δ
    ///     - deterministic: Q x ∑ → Q
    ///     - nondeterministic: Q x Q → ∑
    /// \attention logic could be overridden
    const std::vector<uint32_t> m_trans_table = {};

};

} // namespace automates
