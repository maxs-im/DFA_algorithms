/// \author Maksym Halchenko
/// \github @maxs-im
/// \date Year 2020

#include <fstream>

#include <cmath>

#include "dfs/nested.hpp"
#include "dfs/two_stack.hpp"
#include "utils/converters.hpp"
#include "utils/representation.hpp"
#include "utils/generator.hpp"
#include "statistic.hpp"

#include "LightweightParsingCMD.hpp"
#include "TextTable.h"

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
'emptiness_dfs' usage:\n\
\n\
--generator     [number > 0]    enable generation mode. See more info by calling it with help parameter;\n\
--help          [NONE/bool]     Show info about this binary (programm);\n\
--nba           [NONE/bool]     Works only with NBA (converts NGA if needed);\n\
--nested        [NONE/bool]     Invokes Nested algorithm. We use Two-stack by default (because optimal);\n\
--in_file       [text]          Input file name where we store interested automaton;\n\
--out_file      [text]          Output file name where we will dump converted automaton (if will exist);\n\
************************\n\
Return true or false for selected algorithm\n\
\n\
Author: Maksym Halchenko\n\
    Github: @maxs-im\n\
Year: 2020\n\
\n";

/// \brief Program generate mode description
constexpr static const char* GENERATE_INFO_TEXT = "\
Statistic generator. Could show comparative characteristic for all methods\n\
        \"--generation\" parameter will set up number of repetition for each approach. 0 - means disabled\n\
\n\
There are some additional parameteres to work with generation mode. \n\
\n\
'emptiness_dfs --generated 1' usage:\n\
\n\
--help          [NONE/bool]     Show info about this this generated mode;\n\
--states        [number]        Number of power of 10 for automaton states. Will be generated 10^0, 10^1,..,10^n\n\
--alphabet      [number]        Number of symbols in alphabet. Also, alphabet is [1, alphabet];\n\
--trees         [number]        Number of generated trees for the transition table. \
                                    Default value is a minimal for really similar table (2);\n\
--sets          [number]        Number of sets in final states container. Default value for NBA (1);\n\
--edges         [number]        Number of the edges for each (not leaf and not pre-leaf) vertex in the tree.\
                                    Default value for binary tree (2);\n\
--out_file      [text]          Output file name where we will dump generated information;\n\
************************\n\
\n\
Author: Maksym Halchenko\n\
    Github: @maxs-im\n\
Year: 2020\n\
\n";

/// \struct options It store command line arguments
struct options
{
    /// \brief enable generation mode. Also, value will repeat execution for each configured automaton
    automates::buchi::atm_size generator = 0;
    /// \brief Show info about this binary (programm)
    bool help = false;
    /// \brief Works only with NBA (converts NGA if needed)
    bool nba = false;
    /// \brief Invokes Nested algorithm. We use Two-stack by default (because optimal)
    bool nested = false;
    /// \brief Input file name where we store interested automaton
    std::string in_file = "test_dfs.txt";
    /// \brief Output file name where we will dump converted automaton (if will exist)
    std::string out_file = "dump.txt";
};

/// \brief Read input automaton from file (if it exists) otherwise from console
/// \param name: input file name
/// \return automaton
automates::buchi proceed_data(const std::string& name) noexcept
{
    std::istream *in = &std::cin;
    std::ifstream fs;
    if (!name.empty())
    {
        fs.open(name, std::fstream::in);
        if (!fs.is_open())
            std::cerr << "Failed to read from " << name << " file!\n";
        else
            in = &fs;
    }

    auto automat = utils::representation::construct_read(*in);
    std::cout << "Successfully read automaton\n";
    return automat;
}

/// \brief Help to convert automaton to the NBA and dump logs if needed
/// \param automaton: automaton for the conversation
/// \param name: output file name. Dumping will be ignored on incorrect file
/// \return converted automaton or nullopt in case of redundant conversion
std::optional<automates::buchi> proceed_conversion(const automates::buchi& automaton, const std::string& name) noexcept
{
    auto converted_automat = utils::converters::nga2nba(automaton);
    // no conversion is needed
    if (!converted_automat)
        return converted_automat;

    std::cout << "Successfully converted to NGA\n";

    if (!name.empty())
    {
        std::ofstream fs(name, std::fstream::out);
        if (!fs.is_open())
            std::cerr << "Failed to dump to " << name << " file!\n";
        else
        {
            fs << *converted_automat;
            std::cout << "Successfully dumped into the " << name << " file\n";
        }
    }

    return converted_automat;
}

/// \brief Handle default usage case: calculation of the input automaton
/// \param opts: parsed command line options
void handle_user_case_call(const options& opts) noexcept
{
    auto automaton = proceed_data(opts.in_file);
    // need to convert in case of Nested algorithm for NGA
    auto nba_automaton = (opts.nba || opts.nested) ?
                          std::move(proceed_conversion(automaton, opts.out_file)) :
                          std::nullopt;

    // Return reference on the work automaton (to prevent redundant copying)
    auto get_worker = [&automaton, &nba_automaton]() -> const auto&
                      { return nba_automaton ? *nba_automaton : automaton; };


    using namespace emptiness_check::dfs;
    std::cout << std::boolalpha << "...\n";
    if (opts.nested)
        std::cout << "Nested: " << nested::is_empty(get_worker()) << "\n";
    else
        std::cout << "Two-stack (" << (get_worker().is_generalized() ? "NGA" : "NBA") << "): " <<
                  two_stack::is_empty(get_worker()) << "\n";
}

/// \brief Print user-friendly statistic table
/// \param stats: generated statistic
/// \param name: out file name. Otherwise console will be used
void print_statistic(const std::vector<emptiness_check::statistic::one_step>& stats, const std::string& name) noexcept
{
    std::ostream *out = &std::cout;
    std::ofstream fs;
    if (name.empty())
    {
        fs.open(name, std::fstream::out);
        if (!fs.is_open())
            std::cerr << "Failed to dump to " << name << " file!\n";
        else
            out = &fs;
    }

    TextTable t;
    t.addRow(std::vector<const char *>{"States", "Repetition", "Av. conversation", "Av. generation",
            "NESTED", "TWO-STACK NBA", "TWO-STACK NGA", "Different results"});

    for (auto& stat : stats)
    {
        using namespace emptiness_check::statistic;
        auto create_word = [](std::optional<automates::buchi::atm_size> num, const call_durration& durr)
        {
            return std::to_string(durr.count()) + "ms" + (num ? " (" + std::to_string(*num) + ")" : "");
        };

        std::vector<std::string> container{std::to_string(stat.states), std::to_string(stat.repetition),
                                  create_word(stat.average_conversion.first, stat.average_conversion.second),
                                              create_word(std::nullopt, stat.average_generation)};
        container.reserve(container.size() + 4);

        for (auto& [num, durr] : stat.average_nba)
            container.emplace_back(create_word(num, durr));
        for (auto& [num, durr] : stat.average_nga)
            container.emplace_back(create_word(num, durr));

        std::string text;
        for (auto& it : stat.different_results)
        {
            std::string warning = ":";

            for (auto sit : it.first)
                if (sit != it.first[0])
                    warning += "-NBA";
            for (auto sit : it.second)
                if (sit != it.second[0])
                    warning += "-NGA";
            if (!it.second.empty() && !it.first.empty() &&
                 it.first[0] != it.second[0])
                warning += "-both";
            text += warning;
        }

        container.emplace_back(text);
        t.addRow(container);
    }

    t.setAlignment( 0, TextTable::Alignment::RIGHT );

    *out << t;
}

/// \brief Handle generation logic for user
/// \opts: options from the main(default or calculation) mode
/// \gen_opts: generation mode options
void handle_generator_call(const options& opts, utils::generator::generator_opts gen_opts) noexcept
{
    const emptiness_check::statistic::callbacks_handler<automates::buchi> callbacks = {
            .generation_fn = [&gen_opts]() { return utils::generator::generate_automaton(gen_opts); },
            .conv_fn = &utils::converters::nga2nba,
            .nba_algorithms = {
                    &emptiness_check::dfs::nested::is_empty,
                    &emptiness_check::dfs::two_stack::is_empty
            },
            .nga_algorithms = {
                    &emptiness_check::dfs::two_stack::is_empty
            }
    };

    std::vector<emptiness_check::statistic::one_step> stats;
    // No sense take bigger. UINTMAX < 10^10
    auto max_opt_states = std::min(static_cast<uint32_t>(gen_opts.states), 9u);
    for (automates::buchi::atm_size i = 0; i <= max_opt_states; ++i)
    {
        // note to update opts (reference inside generator callback)
        gen_opts.states = std::pow(10, i);
        auto statistic = one_step_generation_dfs(opts.generator, callbacks);
        statistic.states = gen_opts.states;

        stats.emplace_back(statistic);
    }

    print_statistic(stats, opts.out_file);
}

/// \brief Program entry point: parsing arguments, running selected program mode
/// \details Four operating modes:
///     - README or help
///     - Automaton calculation
///     - Generation calculation statistic
///     - Generation README or help
int main(int argc, const char *argv[])
{
    auto parser = CmdOpts<options>::Create({
        {"--generator", &options::generator},
        {"--help", &options::help},
        {"--nba", &options::nba},
        {"--nested", &options::nested},
        {"--in_file", &options::in_file},
        {"--out_file", &options::out_file},
    });
    auto opts = parser->parse(argc, argv);

    if (opts.generator)
    {
        if (opts.help)
            std::cout << GENERATE_INFO_TEXT;
        else
        {
            using namespace utils::generator;
            auto gen_parser = CmdOpts<generator_opts>::Create({
                {"--states", &generator_opts::states},
                {"--alphabet", &generator_opts::alphabet},
                {"--trees", &generator_opts::trees},
                {"--sets", &generator_opts::sets},
                {"--edges", &generator_opts::edges},
            });
            auto generate_opts = gen_parser->parse(argc, argv);

            handle_generator_call(opts, generate_opts);
        }
    }
    else
    {
        if (argc == 1 || opts.help)
            std::cout << INFO_TEXT;
        else
            handle_user_case_call(opts);
    }

    return 0;
}
