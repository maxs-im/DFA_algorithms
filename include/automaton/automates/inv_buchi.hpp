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
    std::optional<table_container::const_iterator> acceptable_inv_transitions(uint32_t state) const noexcept;

private:
    const table_container m_trans_table_inv = {};

    static table_container inverse_trans_table(const table_container &trans_table) noexcept;

};

} // namespace automates
