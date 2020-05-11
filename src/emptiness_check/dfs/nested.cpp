#include "dfs/nested.hpp"

#include <bitset>

namespace emptiness_check::dfs::nested
{

/// \typedef to storing DFS visiting info: <state, <first entrance bit, final state mark entrance>>
using um = std::unordered_map<uint32_t, std::bitset<2>>;
/// \typedef to storing states of the path
using us = std::unordered_set<uint32_t>;

/// \namespace Anonymous namespace. Helpers with DFS steps
namespace
{

/// \brief Check if q is reachable from itself. Will notify NONEMPTY
/// \param q: the state in which we are now
/// \param S: DFS state visiting info
/// \param P: current story of the state of the path
/// \param automat: investigated automat
/// \return true if we have to continue investigation
bool dfs2(const uint32_t q, um& S, us& P, const automates::buchi &automat) noexcept
{
    S[q].set(1);

    if (const auto &acceptor = automat.acceptable_transitions(q); acceptor)
    {
        for (const auto&[r, _] : acceptor.value()->second)
        {
            if (P.find(r) != P.end())
                return false; // NONEMPTY
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
/// \param S: DFS state visiting info
/// \param P: current story of the state of the path
/// \param automat: investigated automat
/// \return true if we have to continue investigation
bool dfs1(const uint32_t q, um& S, us& P, const automates::buchi &automat) noexcept
{
    S[q].set(0);
    P.insert(q);

    if (const auto &acceptor = automat.acceptable_transitions(q); acceptor)
    {
        for (const auto&[r, _] : acceptor.value()->second)
            if (const auto &it_bits = S.find(r);
                    it_bits == S.end() || !it_bits->second.test(0))
            {
                if (!dfs1(r, S, P, automat))
                    return false;
            }
    }
    // Note: better to add 0 due to NBA
    if (automat.is_final(q))
        if (!dfs2(q, S, P, automat))
            return false;

    P.erase(q);

    return true;
}

}

bool is_empty(const automates::buchi &automat) noexcept
{
    assert(!automat.is_generalized() && "NGA unsupported");

    um S; us P;
    return dfs1(automat.get_initial_state(), S, P, automat);
}

} // namespace emptiness_check::dfs::nested
