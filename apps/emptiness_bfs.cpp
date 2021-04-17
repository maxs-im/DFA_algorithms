#include "include/emptiness_cmd_helper.hpp"
#include "utils/converters.hpp"

#include "bfs/emerson.hpp"
#include "bfs/modified_emerson.hpp"

/// \brief Author info
constexpr static const char* AUTHOR_TEXT = "\
\n\
Author: Maksym Mykhailenko\n\
    Github: @karlion\n\
Year: 2020\n\
\n";

/// \brief Program calculation mode description
constexpr static const char* INFO_TEXT = "\
Emptiness check: Algorithms based on breadth-first search \n\
\n\
We present two emptiness algorithms that explore A using breadth-first search (BFS):\n\
    1. Emerson-Lei -- used by default for both automatone types (NBA/NGA)\n\
    2. Modified Emerson-Lei - used by NBA only\n\
 NBA -- nondeterministic Büchi automaton;\n\
 NGA -- nondeterministic Generalized Büchi automaton;\n\
This program also provide NGA-to-NBA converter. For greater compatibility.\n\
\n\
'THIS_BINARY' usage:\n\
\n\
--generator         [number > 0]    enable generation mode. See more info by calling it with help parameter;\n\
--help              [NONE/bool]     Show info about this binary (programm);\n\
--nba               [NONE/bool]     Works only with NBA (converts NGA if needed);\n\
--modified          [NONE/bool]     Invokes modified algorithm.\n\
--in_file           [text]          Input file name where we store interested automaton;\n\
--out_file          [text]          Output file name where we will dump converted automaton (if exists);\n\
************************\n\
Return true or false for selected algorithm\n\
\n";

/// \brief Handle default usage case: calculation of the input automaton
/// \param opts: parsed command line options
void handle_user_case_call(const emptiness_cmd_helper::options& opts) noexcept
{
    using namespace emptiness_cmd_helper;
    auto automaton = proceed_data(opts.in_file);
    auto nba_automaton = (opts.nba || opts.modified) ?
                         std::move(proceed_conversion(automaton, opts.out_file)) :
                         std::nullopt;

    // Return reference on the work automaton (to prevent redundant copying)
    auto get_worker = [&automaton, &nba_automaton]
        { return automates::inv_buchi(std::move(nba_automaton ? *nba_automaton : automaton)); };

    using namespace emptiness_check::bfs;
    std::cout << std::boolalpha << "...\n";
    if (opts.modified)
        std::cout << "Modified Emerson: " << modified_emerson::is_empty(get_worker()) << "\n";
    else
        std::cout << "Emerson: " << emerson::is_empty(get_worker()) << "\n";
}

/// \brief Initialize callbacks for generation
/// \param opts: generation options that will be passed for appropriate callback
/// \return callbacks handler object
inline emptiness_check::statistic::callbacks_handler<automates::inv_buchi> intialize_callbacks(
        const utils::generator::generator_opts &opts) noexcept
{
    return {
            .generation_fn = [&opts]() { return automates::inv_buchi(utils::generator::generate_automaton(opts)); },
            .conv_fn = [](const automates::inv_buchi& at) -> std::optional<automates::inv_buchi>
            {
                /// \note Yes, slicing
                auto nba = utils::converters::nga2nba(static_cast<automates::buchi>(at));
                return nba ?
                    std::make_optional(automates::inv_buchi(std::move(*nba))) :
                    std::nullopt;
            },
            .nba_algorithms = {
                    &emptiness_check::bfs::emerson::is_empty,
                    &emptiness_check::bfs::modified_emerson::is_empty
            },
            .nga_algorithms = {
                    &emptiness_check::bfs::emerson::is_empty
            }
    };
}

/// \brief Program entry point: parsing arguments, running selected program mode
/// \param argc: the number of command line arguments
/// \param argv: list of command-line arguments
int main(int argc, const char *argv[])
{
    emptiness_cmd_helper::command_line<automates::inv_buchi>(argc, argv,
                        { AUTHOR_TEXT, INFO_TEXT,
                          {"NBA EMERSON", "NGA EMERSON" },
                          &handle_user_case_call, &intialize_callbacks });
    return 0;
}
