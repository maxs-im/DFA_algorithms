#include "utils/converters.hpp"

#include <queue>

namespace utils::converters
{
using namespace automates::buchi;

/// \namespace Anonymous namespace. Helpers with paired states
namespace
{

/// \typedef helper for conversion
using pr = std::pair<uint32_t, uint32_t>;
/// \struct helper for using pairs in unordered structs
struct pr_hasher
{
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const &pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 ^ h2;
    }
};

/// \brief Translate unique paired states into the numbered and constructs new NBA
/// \param Q: all paired states
/// \param Q0: initial paired state
/// \param F: final states (one set) in paired states
/// \param delta: transition table in paired states
/// \return new NBA automaton (not generalized)
nba decode_paired_states(const std::unordered_set<pr, pr_hasher> &Q, const pr &Q0,
                         const std::unordered_set<pr, pr_hasher> &F,
                         const std::unordered_map<pr, std::unordered_map<pr, uint32_t, pr_hasher>, pr_hasher> &delta)
{
    const auto states_num = Q.size();

    std::unordered_map<pr, uint32_t, pr_hasher> dict;
    dict.reserve(states_num);
    uint32_t state_num = 0;
    for (const auto& it : Q)
        dict[it] = state_num++;

    std::unordered_set<uint32_t> new_F;
    new_F.reserve(F.size());
    for (const auto& itF : F)
        new_F.insert(dict[itF]);

    nba::table_container new_delta;
    new_delta.reserve(delta.size());
    for (const auto& [old_st, map] : delta)
        for (const auto& [new_st, sym] : map)
            new_delta[dict[old_st]][dict[new_st]] = sym;

    return nba(states_num, dict[Q0], { std::move(new_F) }, std::move(new_delta));
}

} // namespace anonymous

nba nga2nba(const nba& automat) noexcept
{
    if (automat.is_generalized())
        return automat;

    // state and final state new containers
    std::unordered_set<pr, pr_hasher> Q, F;
    // container for the new transition table
    std::unordered_map<pr, std::unordered_map<pr, uint32_t, pr_hasher>, pr_hasher> delta;
    // new initial point
    pr Q0 {automat.get_initial_state(), 0};

    // queue tracker for new generated state
    std::queue<pr> W;
    W.push(Q0);
    while (!W.empty())
    {
        // current tracked new state
        auto pair = W.front();
        auto& [q, i] = pair;
        W.pop();

        Q.insert(pair);
        if (automat.is_final(q, 0) && i == 0)
            F.insert(pair);

        if (const auto &acceptor = automat.acceptable_transitions(q); acceptor)
            for (const auto& [qt, a] : acceptor.value()->second)
            {
                // generate either new or current states copy
                pr new_pair {qt, automat.is_final(q, i) ? (i + 1) % automat.get_final_num_sets() : i};
                if (Q.find(new_pair) == Q.end())
                    W.push(new_pair);

                // accepted state for current tracked state
                delta[pair][new_pair] = a;
            }
    }

    return decode_paired_states(Q, Q0, F, delta);
}

} // namespace utils::converters
