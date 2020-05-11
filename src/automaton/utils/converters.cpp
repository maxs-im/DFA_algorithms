#include "utils/converters.hpp"

#include <queue>

namespace utils::converters
{
using namespace automates;

/// \namespace Anonymous namespace. Helpers with paired states
namespace
{

/// \typedef helper for conversion
using pr = std::pair<uint32_t, uint32_t>;
/// \struct helper for using pairs in unordered structs
struct pr_hasher
{
    std::size_t operator()(const std::pair<uint32_t, uint32_t> &pair) const
    {
        std::size_t h1 = std::hash<uint32_t>()(pair.first);
        std::size_t h2 = std::hash<uint32_t>()(pair.second);

        return h1 ^ h2;
    }
};

/// \brief Translate unique paired states into the numbered and constructs new NBA
/// \param Q: all paired states
/// \param Q0: initial paired state
/// \param F: final states (one set) in paired states
/// \param delta: transition table in paired states
/// \return new NBA automaton (not generalized)
buchi decode_paired_states(const std::unordered_set<pr, pr_hasher> &Q, const pr &Q0,
                           const std::unordered_set<pr, pr_hasher> &F,
                           const std::unordered_map<pr, std::unordered_map<pr, uint32_t, pr_hasher>, pr_hasher> &delta)
{
    std::unordered_map<pr, uint32_t, pr_hasher> dict;
    dict.reserve(Q.size());

    uint32_t state_num = 0;
    for (const auto& it : Q)
        // reserve default initial state value correctly for the new automaton
        if (it == Q0)
            dict[it] = automates::buchi::INITIAL_STATE;
        else
            dict[it] = state_num == automates::buchi::INITIAL_STATE ? ++state_num : state_num++;

    std::unordered_set<uint32_t> new_F;
    new_F.reserve(F.size());
    for (const auto& itF : F)
        new_F.insert(dict[itF]);

    buchi::table_container new_delta;
    new_delta.reserve(delta.size());
    for (const auto& [old_st, map] : delta)
        for (const auto& [new_st, sym] : map)
            new_delta[dict[old_st]][dict[new_st]] = sym;

    return buchi({std::move(new_F) }, std::move(new_delta));
}

} // namespace anonymous

buchi nga2nba(const buchi& automat) noexcept
{
    if (!automat.is_generalized())
        return automat;

    // state and final state new containers
    std::unordered_set<pr, pr_hasher> Q, F;
    // container for the new transition table
    std::unordered_map<pr, std::unordered_map<pr, uint32_t, pr_hasher>, pr_hasher> delta;
    // new initial point
    pr Q0 {automates::buchi::INITIAL_STATE, 0};

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
