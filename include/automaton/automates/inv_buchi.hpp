#pragma once

#include "automates/buchi.hpp"

namespace automates
{

// TODO: Karlion refactor
class inv_buchi : public buchi
{
public:
    explicit inv_buchi(buchi&& automat) noexcept;

    [[nodiscard]]
    std::optional<table_container::const_iterator> acceptable_inv_transitions(atm_size state) const noexcept;

    const std::unordered_set<atm_size> m_set_of_states = {};
private:
    const table_container m_trans_table_inv = {};

    static table_container inverse_trans_table(const table_container &trans_table) noexcept;

    static std::unordered_set<atm_size> collect_states(const table_container &trans_table) noexcept;

};

} // namespace automates
