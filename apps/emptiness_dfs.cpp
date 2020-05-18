/// \author Maksym Halchenko
/// \github @maxs-im
/// \date Year 2020

#include "include/emptiness_cmd_helper.hpp"

#include "dfs/nested.hpp"
#include "dfs/two_stack.hpp"

/// \brief Author info
constexpr static const char* AUTHOR_TEXT = "\
\n\
Author: Maksym Halchenko\n\
    Github: @maxs-im\n\
Year: 2020\n\
\n";

/// \brief Program calculation mode description
constexpr static const char* INFO_TEXT = "\
Emptiness check: Algorithms based on depth-first search \n\
\n\
We present two emptiness algorithms that explore A using depth-first search (DFS):\n\
    1. Nested -- used by NBA only\n\
    2. Two-stacked - used by default for both automatone types (NBA/NGA)\n\
 NBA -- nondeterministic Büchi automaton;\n\
 NGA -- nondeterministic Generalized Büchi automaton;\n\
This program also provide NGA-to-NBA converter. For greater compatibility.\n\
\n\
The AIM of the program is to answer if A is nonempty and if it has an accepting lasso!\n\
\n\
'THIS_BINARY' usage:\n\
\n\
--generator         [number > 0]    enable generation mode. See more info by calling it with help parameter;\n\
--help              [NONE/bool]     Show info about this binary (programm);\n\
--nba               [NONE/bool]     Works only with NBA (converts NGA if needed);\n\
--non_optimal_only  [NONE/bool]     Invokes Nested algorithm. We use Two-stack by default (because optimal);\n\
--in_file           [text]          Input file name where we store interested automaton;\n\
--out_file          [text]          Output file name where we will dump converted automaton (if will exist);\n\
************************\n\
Return true or false for selected algorithm\n\
\n";

/// \brief Handle default usage case: calculation of the input automaton
/// \param opts: parsed command line options
void handle_user_case_call(const emptiness_cmd_helper::options& opts) noexcept
{
    using namespace emptiness_cmd_helper;
    auto automaton = proceed_data(opts.in_file);
    // need to convert in case of Nested algorithm for NGA
    auto nba_automaton = (opts.nba || opts.non_optimal_only) ?
                          std::move(proceed_conversion(automaton, opts.out_file)) :
                          std::nullopt;

    // Return reference on the work automaton (to prevent redundant copying)
    auto get_worker = [&automaton, &nba_automaton]() -> const auto&
                      { return nba_automaton ? *nba_automaton : automaton; };


    using namespace emptiness_check::dfs;
    std::cout << std::boolalpha << "...\n";
    if (opts.non_optimal_only)
        std::cout << "Nested: " << nested::is_empty(get_worker()) << "\n";
    else
        std::cout << "Two-stack (" << (get_worker().is_generalized() ? "NGA" : "NBA") << "): " <<
                  two_stack::is_empty(get_worker()) << "\n";
}

/// \brief Initialize callbacks for generation
/// \param opts: generation options that will be passed for appropriate callback
/// \return callbacks handler object
inline emptiness_check::statistic::callbacks_handler<automates::buchi> intialize_callbacks(
        const utils::generator::generator_opts &opts) noexcept
{
    return {
        .generation_fn = [&opts] { return utils::generator::generate_automaton(opts); },
        .conv_fn = &utils::converters::nga2nba,
        .nba_algorithms = {
                &emptiness_check::dfs::nested::is_empty,
                &emptiness_check::dfs::two_stack::is_empty
        },
        .nga_algorithms = {
                &emptiness_check::dfs::two_stack::is_empty
        }
    };
}

/// \brief Program entry point: parsing arguments, running selected program mode
/// \param argc: the number of command line arguments
/// \param argv: list of command-line arguments
int main(int argc, const char *argv[])
{
    emptiness_cmd_helper::command_line<automates::buchi>(argc, argv,
                        { AUTHOR_TEXT, INFO_TEXT, &handle_user_case_call, &intialize_callbacks });
    return 0;
}
