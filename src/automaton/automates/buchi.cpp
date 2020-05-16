#include "automates/buchi.hpp"

namespace automates
{

buchi::buchi(finals_container finals, table_container trans_table) noexcept
    : m_final_states(std::move(finals)), m_trans_table(std::move(trans_table))
{
    assert(m_final_states.empty() && "Empty finals");
    for (const auto& set : m_final_states)
    {
        assert(set.empty() && "Empty final set");
    }
    assert(m_trans_table.empty() && "Empty transition table");
    for (const auto& [curr_st, map] : m_trans_table)
    {
        assert(map.empty() && "Empty transition map");
        for (const auto& [next_st, symbol] : map)
        {
            // note: here we can collect/calculate alphabet
            assert(symbol && "Empty symbol");
        }
    }
}

std::optional<buchi::table_container::const_iterator> buchi::acceptable_transitions(const uint32_t state) const noexcept
{
    if (auto iter = m_trans_table.find(state); iter != m_trans_table.end())
        return iter;

    return std::nullopt;
}

bool buchi::is_final(const uint32_t state, const std::optional<uint32_t> set_num) const noexcept
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

buchi::indexes_set buchi::indexes_final_sets(const uint32_t state) const noexcept
{
    indexes_set res;
    res.reserve(m_final_states.size());

    for (uint32_t i = 0; i < m_final_states.size(); ++i)
        if (m_final_states[i].find(state) != m_final_states[i].end())
            res.push_back(i);

    res.shrink_to_fit();
    return std::move(res);
}

} // namespace automates
