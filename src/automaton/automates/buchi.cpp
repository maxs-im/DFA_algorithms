#include "automates/buchi.hpp"

#include <queue>

namespace automates::buchi
{

/// \namespace Anonymous namespace. Helpers with reading and paired states
namespace
{
/// \brief Read final states in format: sets num, {in_set_num, {num}x(in_set_num)}x(sets num)
/// \param in: input stream
/// \return container for final states (nba::finals_container)
auto read_final_states(std::istream &in) noexcept
{
    nba::finals_container final_states;  // container for accept states
    uint32_t fs_num;    // number of sets
    // read number of sets
    in >> fs_num;
    final_states.reserve(fs_num);
    for (; fs_num; --fs_num)
    {
        std::unordered_set<uint32_t> final_states_in_set;   // container for one accept set with final states
        uint32_t fs_set; // number of elements in current set
        // read amount in current set
        in >> fs_set;
        final_states_in_set.reserve(fs_set);
        for (; fs_set; --fs_set)
        {
            uint32_t x; // final state
            // read final state
            in >> x;
            final_states_in_set.insert(x);
        }
        if (!final_states_in_set.empty())
            final_states.push_back(final_states_in_set);
    }

    return std::move(final_states);
}

/// \brief Read trasition table in format Q x Q → ∑
/// \param in: input stream
/// \param states: number of unique states
/// \return container for the transition table (nba::table_container)
auto read_transition_table(std::istream &in, const uint32_t states) noexcept
{
    nba::table_container table; // container for transition table
    table.reserve(states);
    for (uint32_t st = 0; st < states; ++st)
    {
        std::unordered_map<uint32_t, uint32_t> transitions; // transitions for current state
        transitions.reserve(states);

        for (uint32_t in_st = 0; in_st < states; ++in_st)
        {
            uint32_t x; // symbol for transition from st to in_st
            // read symbol
            in >> x;
            if (x == 0) continue;   // empty symbol. Ignore this transition
            transitions.insert({ in_st, x });
        }
        if (!transitions.empty())
            table.insert({ st, transitions });
    }

    return std::move(table);
}

/// \brief Read input until EOF by three numbers: current state, symbol, acceptable state
/// \param in: input stream
/// \return container for final states (nba::finals_container)
auto read_transition_table_paired(std::istream &in) noexcept
{
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> table; // container for transition table

    while (!in.eof())
    {
        // read current state with symbol that accepts new state
        uint32_t curr_st, symbol, next_st;
        in >> curr_st >> symbol >> next_st;

        table[curr_st][next_st] = symbol;
    }

    return std::move(table);
}

/// \typedef helper for conversion
using pr = std::pair<uint32_t, uint32_t>;
/// \struct helper for using pairs in unordered structs
struct pair_hasher
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
nba decode_paired_states(const std::unordered_set<pr, pair_hasher> &Q, const pr &Q0,
        const std::unordered_set<pr, pair_hasher> &F,
        const std::unordered_map<pr, std::unordered_map<pr, uint32_t, pair_hasher>, pair_hasher> &delta)
{
    const auto states_num = Q.size();

    std::unordered_map<pr, uint32_t, pair_hasher> dict;
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

nba::nba(const uint32_t states, const uint32_t start, nba::finals_container finals, nba::table_container trans_table
        ) noexcept : m_states(states), m_start(start),
          m_final_states(std::move(finals)), m_trans_table(std::move(trans_table))
{
    assert(m_states && "Empty states");
    assert(m_start < m_states && "Incorrect start/initial point");
    for (const auto& set : finals)
    {
        assert(set.empty() && "Empty final set");
        for (const auto &ac : set)
            assert(ac < m_states && "Incorrect final state");
    }
    for (const auto& [curr_st, map] : trans_table)
    {
        assert(map.empty() && "Empty transition map");
        assert(curr_st < m_states && "Incorrect current state");
        for (const auto& [next_st, symbol] : map)
        {
            // note: here we can collect/calculate alphabet
            assert(symbol && "Empty symbol");
            assert(next_st < m_states && "Incorrect next state");
        }
    }
}

std::optional<nba::table_container::const_iterator> nba::acceptable_transitions(const uint32_t state) const noexcept
{
    if (auto iter = m_trans_table.find(state); iter != m_trans_table.end())
        return iter;

    return std::nullopt;
}

bool nba::is_final(const uint32_t state, const std::optional<uint32_t> set_num) const noexcept
{
    if (set_num)
    {
        // return false on too big final set index
        return *set_num < m_final_states.size() ?
            m_final_states[*set_num].find(state) != m_final_states[*set_num].end() :
            false;
    }

    for (const auto& set : m_final_states)
        if (set.find(state) != set.end())
            return true;

    return false;
}

std::vector<uint32_t> nba::indexes_final_sets(const uint32_t state) const noexcept
{
    std::vector<uint32_t> res;
    res.reserve(m_final_states.size());

    for (uint32_t i = 0; i < m_final_states.size(); ++i)
        if (m_final_states[i].find(state) != m_final_states[i].end())
            res.push_back(i);

    res.shrink_to_fit();
    return res;
}

void nba::print(std::ostream &out) const noexcept
{
    const char* type = is_generalized() ? "NGA" : "NBA";
    out << "\t" << type << " Automaton\n";

    // print final sets
    for (size_t i = 0; i < m_final_states.size(); ++i, out << '\n')
    {
        out << "Final set " << i << ": ";
        for (const auto& it : m_final_states[i])
            out << it << " ";
    }

    // print transition table
    for (const auto& [from, map] : m_trans_table)
    {
        out << "State " << from << " edges:\n";
        for (const auto& [to, symbol] : map)
            out << "\t -" << symbol << "-> " << to << '\n';
    }
}

nba construct_read(std::istream &in, const bool paired) noexcept
{
    uint32_t states,    // number of unique states
             start;     // initial state
    in >> states >> start;

    // container for accept states
    auto final_states = read_final_states(in);
    // container for transition table
    auto table = paired ?
            read_transition_table_paired(in) :
            read_transition_table(in, states);

    return nba(states, start, std::move(final_states), std::move(table));
}

nba nga2nba(const nba& automat) noexcept
{
    if (automat.is_generalized())
        return automat;

    // state and final state new containers
    std::unordered_set<pr, pair_hasher> Q, F;
    // container for the new transition table
    std::unordered_map<pr, std::unordered_map<pr, uint32_t, pair_hasher>, pair_hasher> delta;
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

} // namespace automates::buchi
