#pragma once

#include <vector>
#include <iostream>

namespace dfa
{
struct automaton
{
    automaton() = delete;

    const uint32_t states, start;
    const std::vector<uint32_t> final_states, trans_table;

    explicit automaton(
        uint32_t _states, uint32_t _start,
        std::vector<uint32_t> _final_states,
        std::vector<uint32_t> _trans_table
    );

    uint32_t action(uint32_t curr, uint32_t sym) const noexcept;

    uint32_t get_alphabet_num() const noexcept;

    bool is_final(uint32_t st) const noexcept;

    void print(std::ostream &out);
};

automaton construct_read(std::istream  &in);
}