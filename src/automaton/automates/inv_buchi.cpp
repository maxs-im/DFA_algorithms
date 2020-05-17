#include "automates/inv_buchi.hpp"

namespace automates
{

inv_buchi::inv_buchi(buchi&& automat) noexcept
    : m_trans_table_inv(std::move(inv_buchi::inverse_trans_table(m_trans_table))),
      buchi(automat)
{}

std::optional<buchi::table_container::const_iterator> inv_buchi::acceptable_inv_transitions(const uint32_t state) const noexcept
{
    if (auto iter = m_trans_table_inv.find(state); iter != m_trans_table_inv.end())
        return iter;

    return std::nullopt;
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
