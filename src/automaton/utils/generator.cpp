#include "utils/generator.hpp"

#include <random>
#include <cmath>
#include <stack>

/// \namespace Anonymous namespace. Helpers with generating automaton parts
namespace
{

/// \brief Generate oriented one-connected tree
/// \param states_num: number of vertex in tree
/// \param alphabet: number of different edges
/// \param edges: number of edges for each_vertex
/// \param is_initial: should we left root equal to 0 (initial automaton state) and connect it with all states
/// \return randomized transition table, but consisting of the one tree
automates::buchi::table_container generate_tree(const uint32_t states_num, const uint32_t alphabet,
                                                const uint32_t edges, const bool is_initial = false) noexcept
{
    // check on emptiness
    if (!states_num || !alphabet || !edges)
        return {};

    std::random_device dev;
    std::mt19937 rng(dev());
    // distribution for symbol selection from alphabet
    std::uniform_int_distribution<uint32_t> dist(1, alphabet);

    // store states for visit. Where index - is a number of the turn and value is a state
    std::vector<uint32_t> stotage(states_num);
    // increasing fill elements from 0 to size()
    std::iota(stotage.begin(), stotage.end(), 0);
    // randomize state selection. Left 0 without changes (correct start point)
    std::shuffle(stotage.begin() + static_cast<uint32_t>(is_initial), stotage.end(), rng);

    automates::buchi::table_container tree;
    // fully connected for initial state. Otherwise, tree could be smaller then max
    const uint32_t max_turns = is_initial ? stotage.size() :
                               std::uniform_int_distribution<uint32_t>(1, stotage.size())(rng);
    // usage of a state: visited or not. Index of a @storage
    for (uint32_t turn = 0; turn < max_turns; ++turn)
    {
        // generate edges for current state
        for (uint32_t i = 0; i <= edges && turn < stotage.size() - i; ++i)
        {
            /// \note: symbol may be overwritten during further tree merge
            tree[stotage[turn]][stotage[turn + i]] = dist(rng);
        }
    }

    return std::move(tree);
}

/// \brief Generate final state sets
/// \param states_num: number of states in automaton
/// \param set_num: number of sets in the final container
/// \param ratio: special experimental value to control maximum elements in one set. Use number of edges for each vertex
/// \return randomized final states container
automates::buchi::finals_container generate_finals(const uint32_t states_num, const uint32_t set_num,
                                                   const uint32_t ratio) noexcept
{
    // check on emptiness
    if (!states_num || !set_num || !ratio)
        return {};

    std::random_device dev;
    std::mt19937 rng(dev());

    // Some manual decrease for maximum final states number per set
    std::uniform_int_distribution<uint32_t> dist_set(1,
            std::max(static_cast<double>(states_num) / set_num / ratio, 1.0));
    // Distribution for final states
    std::uniform_int_distribution<uint32_t> dist(0, states_num - 1);
    automates::buchi::finals_container finals(set_num);

    for (auto& set : finals)
    {
        auto max_in_set = dist_set(rng);
        set.reserve(max_in_set);
        for (uint32_t i = 0; i < max_in_set; ++i)
            set.insert(dist(rng));
    }

    return std::move(finals);
}

} // namespace anonymous

automates::buchi utils::generator::generate_automaton(const generator_opts& opts) noexcept
{
    automates::buchi::table_container transitions;
    // trees generating and merging
    for (uint32_t tree = 0; tree < opts.trees; ++tree)
    {
        auto gt = generate_tree(opts.states, opts.alphabet, opts.edges, tree == 0);
        // merging trees
        for (auto& [from, map] : gt)
            for (auto& [to, symbol] : map)
                transitions[from][to] = symbol;
    }

    return automates::buchi(generate_finals(opts.states, opts.sets, opts.edges), std::move(transitions));
}
