#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace automates
{

/// \class (Nondeterministic) Büchi automaton
class buchi
{
public:
    buchi() = delete;
    buchi& operator=(const buchi&) = delete;

    /// \brief an initial or start state: q0 ∈ Q
    /// \note Use NULL as default for all automates
    static constexpr uint32_t INITIAL_STATE = 0;

    /// \typedef Container definition of the final states struct
    using finals_container = std::vector<std::unordered_set<uint32_t>>;
    /// \typedef Container definition of the transition table struct
    using table_container = std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>>;
    /// \typedef Container definition of the final set indexes
    using indexes_set = std::vector<uint32_t>;

    /// \brief Creates an Büchi object with simple input verification (by asserting)
    /// \param finals: a correct set of sets of final states
    /// \param trans_table: a correct transition table (map<state, map<next_state, symbol>>)
    explicit buchi(finals_container finals, table_container trans_table) noexcept;

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
    [[nodiscard]] indexes_set indexes_final_sets(uint32_t state) const noexcept;

    /// \brief Simple and user-friendly Büchi automaton representation
    ///     "-NUM->" - means NUM symbol for acceptable transition
    ///     \file representation.cpp
    /// \param out: output stream
    /// \param automaton: automaton that will be printed
    /// \return output stream
    friend std::ostream& operator<<(std::ostream& out, const buchi& automaton);

private:
    /// \brief container for a set of accept states: f = {F0,..Fm}, Fi ⊆ Q
    const finals_container m_final_states = {};
    /// \note symbol - 0 is EMPTY symbol
    /// \brief container for a transition function: δ: Q x Q → ∑
    const table_container m_trans_table = {};

};

} // namespace automates