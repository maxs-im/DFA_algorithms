#include "utils/converters.hpp"

#include <queue>

/// \brief hash_combine you could hash an entire (ordered) container, for example, as long as each member
/// is individually hashable
/// \note Boost realization
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
}
/// \struct helper for using pairs in unordered structs
template<typename S, typename T>
struct std::hash<std::pair<S, T>>
{
    std::size_t operator()(const std::pair<S, T> &pair) const
    {
        size_t seed = 0;
        ::hash_combine(seed, pair.first);
        ::hash_combine(seed, pair.second);

        return seed;
    }
};

namespace utils::converters
{

using namespace automates;

/// \namespace Anonymous namespace. Helpers with paired states
namespace
{

/// \typedef helper for conversion
/// \note need to be the same with std::hash
using pr = std::pair<buchi::atm_size, buchi::atm_size>;

/// \brief Translate unique paired states into the numbered and constructs new NBA
/// \param Q: all paired states
/// \param Q0: initial paired state
/// \param F: final states (one set) in paired states
/// \param delta: transition table in paired states
/// \return new NBA automaton (not generalized)
buchi decode_paired_states(const std::unordered_set<pr> &Q, const pr &Q0,
                           const std::unordered_set<pr> &F,
                           const std::unordered_map<pr, std::unordered_set<pr>> &delta)
{
    std::unordered_map<pr, buchi::atm_size> dict;
    dict.reserve(Q.size());

    buchi::atm_size state_num = 0;
    for (const auto& it : Q)
        // reserve default initial state value correctly for the new automaton
        if (it == Q0)
            dict[it] = automates::buchi::INITIAL_STATE;
        else
            dict[it] = state_num == automates::buchi::INITIAL_STATE ? ++state_num : state_num++;

    std::unordered_set<buchi::atm_size> new_F;
    new_F.reserve(F.size());
    for (const auto& itF : F)
        new_F.insert(dict[itF]);

    buchi::table_container new_delta;
    new_delta.reserve(delta.size());
    for (const auto& [old_st, set] : delta)
        for (const auto& new_st : set)
            new_delta[dict[old_st]].insert(dict[new_st]);

    return buchi({std::move(new_F) }, std::move(new_delta));
}

} // namespace anonymous

std::optional<buchi> nga2nba(const buchi& automat) noexcept
{
    if (!automat.is_generalized())
        return std::nullopt;

    // state and final state new containers
    std::unordered_set<pr> Q, F;
    // container for the new transition table
    std::unordered_map<pr, std::unordered_set<pr>> delta;
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

        if (const auto &set = automat.acceptable_transitions(q); set)
            for (const auto& qt : (*set).get())
            {
                // generate either new or current states copy
                pr new_pair {qt, automat.is_final(q, i) ? (i + 1) % automat.get_final_num_sets() : i};
                if (Q.find(new_pair) == Q.end())
                    W.push(new_pair);

                // accepted state for current tracked state
                delta[pair].insert(new_pair);
            }
    }

    return decode_paired_states(Q, Q0, F, delta);
}

} // namespace utils::converters
