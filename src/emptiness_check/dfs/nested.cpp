#include "dfs/nested.hpp"

#include <bitset>
#include <cassert>

namespace emptiness_check::dfs::nested
{

/// \typedef to storing DFS visiting info: <state, <first entrance bit, final state mark entrance>>
using um = std::unordered_map<automates::buchi::atm_size, std::bitset<2>>;
/// \typedef to storing states of the path
using us = std::unordered_set<automates::buchi::atm_size>;

/// \namespace Anonymous namespace. Helpers with DFS steps
namespace
{

/// \brief Check if q is reachable from itself. Will notify NONEMPTY
/// \param q: the state in which we are now
/// \param[in,out] S: DFS state visiting info
/// \param[in,out] P: current story of the state of the path
/// \param automat: investigated automat
/// \return true if we have to continue investigation
bool dfs2(const automates::buchi::atm_size q, um& S, const us& P, const automates::buchi &automat) noexcept
{
    S[q].set(1);

    if (auto iter = automat.m_trans_table.find(q); iter != automat.m_trans_table.end())
    {
        for (const auto &r : iter->second)
        {
            if (P.find(r) != P.end())
                return false; // NONEMPTY NBA
            if (const auto &it_bits = S.find(r);
                    it_bits == S.end() || !it_bits->second.test(1))
            {
                if (!dfs2(r, S, P, automat))
                    return false;
            }
        }
    }

    return true;
}

/// \brief Blackens an accepting state q. Handle @dfs2 notification
/// \param q: the state in which we are now
/// \param[in,out] S: DFS state visiting info
/// \param[in,out] P: current story of the state of the path
/// \param automat: investigated automat
/// \return true if we have to continue investigation
bool dfs1(const automates::buchi::atm_size q, um& S, us& P, const automates::buchi &automat) noexcept
{
    S[q].set(0);
    P.insert(q);

    if (auto iter = automat.m_trans_table.find(q); iter != automat.m_trans_table.end())
    {
        for (const auto &r : iter->second)
            if (const auto &it_bits = S.find(r);
                    it_bits == S.end() || !it_bits->second.test(0))
            {
                if (!dfs1(r, S, P, automat))
                    return false;
            }
    }
    /// \note: better to add 0 due to NBA
    if (automat.is_final(q))
        if (!dfs2(q, S, P, automat))
            return false;

    P.erase(q);

    return true;
}

} // namespace anonymous

bool is_empty(const automates::buchi &automat) noexcept
{
    assert(!automat.is_generalized() && "NGA unsupported");

    um S; us P;
    return dfs1(automates::buchi::INITIAL_STATE, S, P, automat);
}

} // namespace emptiness_check::dfs::nested
