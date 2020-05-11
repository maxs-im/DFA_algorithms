#pragma once

#include <istream>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace automates::buchi
{

/// \class (Nondeterministic) Büchi automaton
class nba
{
public:
    nba() = delete;
    nba& operator=(const nba&) = delete;

    /// \typedef Container definition of the final states struct
    using finals_container = std::vector<std::unordered_set<uint32_t>>;
    /// \typedef Container definition of the transition table struct
    using table_container = std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>>;

    /// \brief Creates an NBA objects with simple input verification (by asserting)
    /// \param states: a number of states.
    /// \param start: a correct initial/start state
    /// \param finals: a correct set of sets of final states
    /// \param trans_table: a correct transition table (map<state, map<next_state, symbol>>)
    explicit nba(uint32_t states, uint32_t start, finals_container finals, table_container trans_table) noexcept;

    /// \brief Getter for a number of unique states
    /// \return maximum state - 1 state number. @m_states value
    [[nodiscard]] uint32_t get_states_num() const noexcept { return m_states; }

    /// \brief Getter for the automaton initial/start state q0
    /// \return @m_start value
    [[nodiscard]] uint32_t get_initial_state() const noexcept { return m_start; }

    /// \brief Get number of final state
    /// \return size of @m_final_states
    [[nodiscard]] uint32_t get_final_num_sets() const noexcept { return m_final_states.size(); }

    /// \brief Check if this is Generalized Büchi automaton
    /// \return true if is NGA (more than one set of acceptable states)
    [[nodiscard]] bool is_generalized() const noexcept { return get_final_num_sets() > 1; }

    /// \brief Get dictionary (map) of possible transitions for the current state
    /// \param state: automaton state of interest
    /// \return map <success state -> symbol to apply>
    [[nodiscard]] std::optional<table_container::const_iterator> acceptable_transitions(uint32_t state) const noexcept;

    /// \brief Check if input number is an accept/final state
    /// \note May be a misunderstanding for NGA
    /// \param state: automaton state
    /// \param set_num: specify final set number. By default check in all
    /// \return whether state belongs to @m_final_states
    [[nodiscard]] bool is_final(uint32_t state, std::optional<uint32_t> set_num = std::nullopt) const noexcept;

    /// \brief Denotes the set of all indices i ∈ K such that state ∈ Fi
    /// \note Sorted ascending and unique
    /// \param state: automaton state
    /// \return Sorted ascending and unique vector of @m_final_states indexes that contain according state
    [[nodiscard]] std::vector<uint32_t> indexes_final_sets(uint32_t state) const noexcept;

    /// \brief Simple and user-friendly dfa representation
    ///     "-NUM->" - means NUM symbol for acceptable transition
    /// \param out: output stream
    void print(std::ostream &out) const noexcept;

private:
    /// \brief a finite set of states Q: (0, num]
    const uint32_t m_states = 0;
    /// \brief an initial or start state: q0 ∈ Q
    const uint32_t m_start = 0;
    /// \brief container for a set of accept states: f = {F0,..Fm}, Fi ⊆ Q
    const finals_container m_final_states = {};
    /// \note symbol - 0 is EMPTY symbol
    /// \brief container for a transition function: δ: Q x Q → ∑
    const table_container m_trans_table = {};

};

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
nba construct_read(std::istream &in, bool paired = true) noexcept;

/// \brief Conversion operation for NGA to NBA automatons
/// \param automat: NGA automaton
/// \return NBA automaton
automates::buchi::nba nga2nba(const automates::buchi::nba& automat) noexcept;

} // namespace automates::buchi
