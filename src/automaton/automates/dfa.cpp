#include "automates/dfa.hpp"

#include <istream>

namespace automates
{

dfa::dfa(const uint32_t states, const uint32_t start, std::vector<uint32_t> finals, std::vector<uint32_t> table)
    : finite_automaton(states, static_cast<uint32_t>(table.size()) / states, start, std::move(finals), std::move(table))
{
    assert(m_trans_table.size() == m_states * m_alphabet && "Incorrect size of the transition table");
    for (const auto &tf : m_trans_table)
        assert(tf < m_states && "Incorrect state in transition table");
}

uint32_t dfa::action(const uint32_t curr, const uint32_t sym) const noexcept
{
    const auto alphabet = m_trans_table.size() / m_states;
    return m_trans_table.at(curr * alphabet + sym);
}

void dfa::print(std::ostream &out) noexcept
{
    out << "\tDFA Automaton\n";
    for (uint32_t st = 0; st < m_states; ++st, out << '\n')
    {
        if (st == m_start)
            out << "->";
        out << st;
        if (is_final(st))
            out << "*";
        out << "\t|";

        for (uint32_t i = 0; i < m_alphabet; ++i, out << '\t')
            out << m_trans_table[st * m_alphabet + i];
    }
}

dfa construct_read(std::istream &in)
{
    uint32_t states, start,
        fs_num, alph_num;
    std::vector<uint32_t> table, final_states;
    in >> states >> start;

    in >> fs_num >> alph_num; 
    final_states.reserve(fs_num);
    table.reserve(alph_num*states);

    for (; fs_num; --fs_num)
    {
        uint32_t x;
        in >> x;
        final_states.push_back(x);
    }

    for (auto i = table.capacity(); i; --i)
    {
        uint32_t x;
        in >> x;
        table.push_back(x);
    }

    return dfa(states, start, std::move(final_states), std::move(table));
}

} // namespace automates
