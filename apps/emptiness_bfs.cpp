#include "include/emptiness_cmd_helper.hpp"
#include "utils/converters.hpp"

#include "bfs/emerson.hpp"

/// \brief Author info
constexpr static const char* AUTHOR_TEXT = "\
NOT ME =)\n\
\n";

/// \brief Program calculation mode description
constexpr static const char* INFO_TEXT = "\
TODO: Add desription\n\
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
    auto get_worker = [&automaton, &nba_automaton]
        { return automates::inv_buchi(std::move(nba_automaton ? *nba_automaton : automaton)); };

    using namespace emptiness_check::bfs;
    std::cout << std::boolalpha << "...\n";
    // TODO: finish it @karlion!
    if (opts.non_optimal_only)
        std::cout << "Emerson: " << emerson::is_empty(get_worker()) << "\n";
    else
        std::cout << "Non-optimal Emerson: " << emerson::is_empty(get_worker()) << "\n";
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
                        { AUTHOR_TEXT, INFO_TEXT, &handle_user_case_call, &intialize_callbacks });
    return 0;
}
