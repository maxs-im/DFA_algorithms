#include "utils/representation.hpp"

#include "automates/buchi.hpp"

#include <istream>

namespace utils::representation
{
using namespace automates;

/// \namespace Anonymous namespace. Helpers with reading automaton parts
namespace
{
/// \brief Read final states in format: sets num, {in_set_num, {num}x(in_set_num)}x(sets num)
/// \param in: input stream
/// \return container for final states (buchi::finals_container)
auto read_final_states(std::istream &in) noexcept
{
    buchi::finals_container final_states;  // container for accept states
    uint32_t fs_num;    // number of sets
    // read number of sets
    in >> fs_num;
    final_states.reserve(fs_num);
    for (; fs_num; --fs_num)
    {
        std::unordered_set<uint32_t> final_states_in_set;   // container for one accept set with final states
        uint32_t fs_set; // number of elements in current set
        // read amount in current set
        in >> fs_set;
        final_states_in_set.reserve(fs_set);
        for (; fs_set; --fs_set)
        {
            uint32_t x; // final state
            // read final state
            in >> x;
            final_states_in_set.insert(x);
        }
        if (!final_states_in_set.empty())
            final_states.push_back(final_states_in_set);
    }

    return std::move(final_states);
}

/// \brief Read trasition table in format Q x Q → ∑
/// \param in: input stream
/// \return container for the transition table (buchi::table_container)
auto read_transition_table(std::istream &in) noexcept
{
    uint32_t states;    // number of unique states
    in >> states;

    buchi::table_container table; // container for transition table
    table.reserve(states);
    for (uint32_t st = 0; st < states; ++st)
    {
        std::unordered_map<uint32_t, uint32_t> transitions; // transitions for current state
        transitions.reserve(states);

        for (uint32_t in_st = 0; in_st < states; ++in_st)
        {
            uint32_t x; // symbol for transition from st to in_st
            // read symbol
            in >> x;
            if (x == 0) continue;   // empty symbol. Ignore this transition
            transitions.insert({ in_st, x });
        }
        if (!transitions.empty())
            table.insert({ st, transitions });
    }

    return std::move(table);
}

/// \brief Read input until EOF by three numbers: current state, symbol, acceptable state
/// \param in: input stream
/// \return container for final states (buchi::finals_container)
auto read_transition_table_paired(std::istream &in) noexcept
{
    buchi::table_container table; // container for transition table

    while (!in.eof())
    {
        // read current state with symbol that accepts new state
        uint32_t curr_st, symbol, next_st;
        in >> curr_st >> symbol >> next_st;

        table[curr_st][next_st] = symbol;
    }

    return std::move(table);
}

} // namespace anonymous

buchi construct_read(std::istream &in, const bool paired) noexcept
{
    // container for accept states
    auto final_states = read_final_states(in);
    // container for transition table
    auto table = paired ?
                 read_transition_table_paired(in) :
                 read_transition_table(in);

    return buchi(std::move(final_states), std::move(table));
}

} // namespace utils::representation

namespace automates
{

std::ostream& operator<<(std::ostream &out, const buchi &automaton)
{
    const char *type = automaton.is_generalized() ? "NGA" : "NBA";
    out << "\t" << type << " Automaton\n";

    // print final sets
    for (size_t i = 0; i < automaton.m_final_states.size(); ++i, out << '\n')
    {
        out << "Final set " << i << ": ";
        for (const auto &it : automaton.m_final_states[i])
            out << it << " ";
    }

    // print transition table
    for (const auto&[from, map] : automaton.m_trans_table)
    {
        out << "State " << from << " edges:\n";
        for (const auto&[to, symbol] : map)
            out << "\t -" << symbol << "-> " << to << '\n';
    }

    return out;
}

} // namespace automates
