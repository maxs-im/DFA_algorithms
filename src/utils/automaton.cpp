#include <automaton.hpp>

#include <sstream>

namespace dfa
{

automaton::automaton(
    const uint32_t _states, const uint32_t _start,
    std::vector<uint32_t> _final_states,
    std::vector<uint32_t> _trans_table
) : states(_states), start(_start), 
    final_states(std::move(_final_states)), 
    trans_table(std::move(_trans_table))  
{
    assert(start < states && "Bad start point");
    assert(trans_table.size() % states == 0 && "Tr.fn is incorrect");
    for (auto tf : trans_table)
        assert(tf < states && "Bad state in transition function");
}

uint32_t automaton::action(const uint32_t curr, const uint32_t sym) const noexcept
{
    const auto alphabet = trans_table.size() / states;
    return trans_table.at(curr * alphabet + sym);
}

uint32_t automaton::get_alphabet_num() const noexcept 
{
    return static_cast<uint32_t>(trans_table.size()) / states;
}

automaton construct_read(std::istream& in) 
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

    for (auto i = alph_num*states; i; --i)
    {
        uint32_t x;
        in >> x;
        table.push_back(x);
    }

    return automaton(states, start, std::move(final_states), std::move(table));
}

bool automaton::is_final(const uint32_t st) const noexcept
{
    const auto &fs = final_states;
    return std::find(fs.begin(), fs.end(), st) != fs.end();
}

void automaton::print(std::ostream &out)
{
    out << "\tDFA Automaton\n";
    for (uint32_t st = 0; st < states; ++st, out << '\n')
    {
        if (st == start)
            out << "->";
        out << st;
        if (is_final(st))
            out << "*";
        out << "\t|";

        const auto alph = get_alphabet_num();
        for (uint32_t i = 0; i < alph; ++i, out << '\t')
            out << trans_table[st * alph + i];
    }
}

} // namespace dfa
