#include "utils/representation.hpp"

#include "automates/buchi.hpp"

#include <iostream>

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
    if (&in == &std::cin)
        std::cout << "Enter number of final sets: ";
    // read number of sets
    in >> fs_num;
    final_states.reserve(fs_num);
    for (; fs_num; --fs_num)
    {
        std::unordered_set<uint32_t> final_states_in_set;   // container for one accept set with final states
        uint32_t fs_set; // number of elements in current set
        if (&in == &std::cin)
            std::cout << "Enter amount of final states for new set: ";
        // read amount in current set
        in >> fs_set;
        final_states_in_set.reserve(fs_set);
        if (&in == &std::cin)
            std::cout << "Enter "<< fs_set << " numbers which will be your final states: ";
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

/// \brief Read input until EOF by two numbers: current state, acceptable state
/// \param in: input stream
/// \return container for final states (buchi::finals_container)
auto read_transition_table(std::istream &in) noexcept
{
    buchi::table_container table; // container for transition table

    const bool is_console = &in == &std::cin;

    uint32_t edges_num = 0;
    if (is_console)
    {
        std::cout << "Enter how many edges will be in your automaton: ";
        in >> edges_num;
    }

    while ((is_console && edges_num--) || (!is_console && !in.eof()))
    {
        if (is_console)
            std::cout << "Enter a pair of vertexes [from, to]: ";
        // read current state with acceptable new state
        uint32_t curr_st, next_st;
        in >> curr_st >> next_st;

        table[curr_st].insert(next_st);
    }

    return std::move(table);
}

} // namespace anonymous

buchi construct_read(std::istream &in) noexcept
{
    // container for accept states
    auto final_states = read_final_states(in);
    // container for transition table
    auto table = read_transition_table(in);

    return buchi(std::move(final_states), std::move(table));
}

} // namespace utils::representation

namespace automates
{
/// \note: Similar to user input
std::ostream& operator<<(std::ostream &out, const buchi &automaton)
{
    // print number of sets
    out << automaton.m_final_states.size() << "\n";
    // print final sets
    for (size_t i = 0; i < automaton.m_final_states.size(); ++i, out << '\n')
    {
        // print number in set
        out << automaton.m_final_states[i].size() << " ";
        for (const auto &it : automaton.m_final_states[i])
            out << it << " ";
    }

    // print transition table
    for (const auto&[from, set] : automaton.m_trans_table)
        for (const auto& to : set)
            out << from << " " << to << '\n';

    return out;
}

} // namespace automates
