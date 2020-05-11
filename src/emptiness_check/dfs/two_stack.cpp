#include "dfs/two_stack.hpp"

#include <stack>
#include <algorithm>

namespace emptiness_check::dfs::two_stack
{

/// \typedef to storing DFS visiting info: <state <V entrance, discovery time>>
using um = std::unordered_map<uint32_t, std::pair<bool, uint32_t>>;
/// \typedef to storing set of candidates: <state, final indexes set for the state>
using si = std::stack<std::pair<uint32_t, automates::buchi::indexes_set>>;

/// \namespace Anonymous namespace. Helpers with DFS steps
namespace
{

/// \brief DFS search with improvements. Will notify NONEMPTY
/// \param q: the state in which we are now
/// \param[in,out] S: DFS state visiting info: <state, <bit whether state in V, state discovery time>>
/// \param[in,out] C: set of candidates, containing the states for which it is not yet known whether they belong to some
/// cycle that denotes the set of all indices i ∈ K such that q ∈ Fi for NGA
/// \param[in,out] V: when a state is discovered (greyed), it is pushed into the stack (so states are always ordered
/// in V by increasing discovery time); and when a root is blackened, all states of V above it (including the root
/// itself) are popped. Note: V ⊆ S holds at all times
/// \param[in,out] t: timestamps for the states
/// \param automat: investigated automat
/// \return true if we have to continue investigation
bool dfs(const uint32_t q, um &S, si &C, std::stack<uint32_t> &V, uint32_t& t, const automates::buchi &automat)
{
    const bool is_nga = automat.is_generalized();
    // To not calculate without a reason indexes set
    C.push({ q, is_nga ? automat.indexes_final_sets(q) : automates::buchi::indexes_set{} });
    V.push(q);
    S[q] = { true, ++t };

    if (const auto &acceptor = automat.acceptable_transitions(q); acceptor)
    {
        for (const auto& [r, _] : acceptor.value()->second)
            if (const auto &it_bits = S.find(r); it_bits == S.end())
            {
                if (!dfs(r, S, C, V, t, automat))
                    return false;
            }
            else if (it_bits->second.first)
            {
                automates::buchi::indexes_set I{};
                uint32_t s;
                do {
                    const auto& [s_top, J] = C.top();
                    s = s_top;
                    if (is_nga)
                    {
                        automates::buchi::indexes_set new_I{};
                        std::set_union(I.begin(), I.end(), J.begin(), J.end(), std::back_inserter(new_I));
                        I = new_I;
                        if (I.size() == automat.get_final_num_sets())
                            return false; // NONEMPTY NGA
                    }
                    else
                    {
                        // Note: may add 0 due to the NBA
                        if (automat.is_final(s))
                            return false; // NONEMPTY NBA
                    }

                    C.pop();
                } while (S[s].second > S[r].second); // lifetime comparing
                C.push({ s, I });
            }
    }

    if (const auto& [c_q, _] = C.top(); c_q == q)
    {
        C.pop();
        uint32_t s;
        do {
            s = V.top();
            V.pop();
            S[s].first = false;
        } while (s != q);
    }

    ++t;
    return true;
}

} // namespace anonymous

bool is_empty(const automates::buchi &automat)
{
    um S;
    si C;
    std::stack<uint32_t> V;
    uint32_t t = 0;

    return dfs(automates::buchi::INITIAL_STATE, S, C, V, t, automat);
}

} // namespace emptiness_check::dfs::two_stack
