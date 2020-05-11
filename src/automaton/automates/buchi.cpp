#include "automates/buchi.hpp"

namespace automates::buchi
{

nba::nba(const uint32_t states, const uint32_t start, nba::finals_container finals, nba::table_container trans_table
        ) noexcept : m_states(states), m_start(start),
          m_final_states(std::move(finals)), m_trans_table(std::move(trans_table))
{
    assert(m_states && "Empty states");
    assert(m_start < m_states && "Incorrect start/initial point");
    for (const auto& set : finals)
    {
        assert(set.empty() && "Empty final set");
        for (const auto &ac : set)
            assert(ac < m_states && "Incorrect final state");
    }
    for (const auto& [curr_st, map] : trans_table)
    {
        assert(map.empty() && "Empty transition map");
        assert(curr_st < m_states && "Incorrect current state");
        for (const auto& [next_st, symbol] : map)
        {
            // note: here we can collect/calculate alphabet
            assert(symbol && "Empty symbol");
            assert(next_st < m_states && "Incorrect next state");
        }
    }
}

std::optional<nba::table_container::const_iterator> nba::acceptable_transitions(const uint32_t state) const noexcept
{
    if (auto iter = m_trans_table.find(state); iter != m_trans_table.end())
        return iter;

    return std::nullopt;
}

bool nba::is_final(const uint32_t state, const std::optional<uint32_t> set_num) const noexcept
{
    if (set_num)
    {
        // return false on too big final set index
        return *set_num < m_final_states.size() ?
            m_final_states[*set_num].find(state) != m_final_states[*set_num].end() :
            false;
    }

    for (const auto& set : m_final_states)
        if (set.find(state) != set.end())
            return true;

    return false;
}

std::vector<uint32_t> nba::indexes_final_sets(const uint32_t state) const noexcept
{
    std::vector<uint32_t> res;
    res.reserve(m_final_states.size());

    for (uint32_t i = 0; i < m_final_states.size(); ++i)
        if (m_final_states[i].find(state) != m_final_states[i].end())
            res.push_back(i);

    res.shrink_to_fit();
    return res;
}

} // namespace automates::buchi
