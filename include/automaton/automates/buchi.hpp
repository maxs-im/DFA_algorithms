#pragma once

#include "nfa.hpp"

namespace automates::buchi
{

class buchi
{
public:

    /// \brief
    /// \return
    bool is_generalized() noexcept const { return m_accept_collection > 1; }

private:
    /// \brief
    const uint32_t m_accept_collection;

};

/// \class Generalized Büchi automaton
class nga : public nfa, public buchi
{
public:

    explicit nga();

    /// \brief Prints Q x Q table with elements means transaction symbol:
    ///     "->" - at the start of the raw means initial state
    ///     "*" - at the start of the raw means final state
    /// \note not displayed same initial|final info for each column
    /// \param out: output stream
    void print(std::ostream &out) noexcept override;

private:
    const uint32_t m_accept_collection;
};

/// \class (Nondeterministic) Büchi automaton
class nba : public nga
{
public:

    explicit nba();

};

} // namespace automates
