#pragma once

#include "automates/buchi.hpp"

namespace utils::generator
{

/// \struct Handle generator options to help with randomization and limitation of the output automaton size
struct generator_opts
{
    /// \brief Number of states for the automaton
    uint32_t states = 0;
    /// \brief Number of symbols in alphabet. Also, alphabet is [1, alphabet].
    /// Such default value due to it does not affect calculation complexity
    uint32_t alphabet = 1;
    /// \brief Number of generated trees for the transition table.
    /// Default value is a minimal for really similar table
    uint32_t trees = 2;
    /// \brief Number of sets in final states container.
    /// Default value for NBA
    uint32_t sets = 2;
    /// \brief Number of the edges for each (not leaf and not pre-leaf) vertex in the tree.
    /// Default value for binary tree
    /// \note: +1 for self-cycle
    uint32_t edges = 2;
};

/// \brief Generate random Buchi automaton. Emptiness will cause assertion
/// \details Generator built on idea of binary trees.
///     We generate @opts.trees with fixed edges (@opts.edges) for each vertex and merge them into one -
///     there is how we get the transition table. Note that 0 (initial state for automaton) will be a root of the
///     first tree, that will be constructed from !all @opts.states. Also, we generate @opts.sets for final states.
///     Number equals to 1 will mean NBA construction. Maximal amount of the states for each set will be
///     !experimentally calculated with this formula: max_in_set = @opts.states / @opts.sets / @opts.edges
/// \param opts: generator options
/// \return random Buchi automaton
automates::buchi generate_automaton(const generator_opts& opts) noexcept;

} // namespace utils::generator
