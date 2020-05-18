#include "automates/inv_buchi.hpp"

#include <cassert>

namespace automates
{

inv_buchi::inv_buchi(buchi&& automat) noexcept
    : m_trans_table_inv(std::move(inv_buchi::inverse_trans_table(m_trans_table))),
      m_set_of_states(collect_states(m_trans_table)),
      buchi(automat)
{
    assert(!is_generalized() && "Need to provide NBA");
}

std::optional<buchi::table_container::const_iterator> inv_buchi::acceptable_inv_transitions(
        const atm_size state) const noexcept
{
    if (auto iter = m_trans_table_inv.find(state); iter != m_trans_table_inv.end())
        return iter;

    return std::nullopt;
}

std::unordered_set<automates::buchi::atm_size> inv_buchi::collect_states(const buchi::table_container &trans_table) noexcept
{
    std::unordered_set<automates::buchi::atm_size> container;
    for (const auto& [from, set] : trans_table)
        for (const auto& to : set)
            container.insert({ from, to });

    return std::move(container);
}

buchi::table_container inv_buchi::inverse_trans_table(const buchi::table_container &trans_table) noexcept
{
    table_container container;
    for (const auto&[q, set] : trans_table)
        for (const auto& qt : set)
            container[qt].insert(q);

    return std::move(container);
}

} // namespace automates
