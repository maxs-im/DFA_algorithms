#pragma once

#include "automates/buchi.hpp"

#include "utils/generator.hpp"
#include "utils/representation.hpp"
#include "utils/converters.hpp"
#include "statistic.hpp"

#include "LightweightParsingCMD.hpp"
#include "TextTable.h"

#include <fstream>
#include <cmath>

/// \namespace Common used functions for the Emptiness-program
namespace emptiness_cmd_helper
{

/// \brief Program generate mode description
constexpr static const char* GENERATE_INFO_TEXT = "\
Statistic generator. Could show comparative characteristic for all methods\n\
        \"--generation\" parameter will set up number of repetition for each approach. 0 - means disabled\n\
\n\
There are some additional parameteres to work with generation mode. \n\
\n\
'THIS_BINARY --generated 1' usage:\n\
\n\
--help          [NONE/bool]     Show info about this this generated mode;\n\
--states        [number]        Number of power of 10 for automaton states. Will be generated 10^0, 10^1,..,10^n\n\
--trees         [number]        Number of generated trees for the transition table. \
                                    Default value is a minimal for really similar table (2);\n\
--sets          [number]        Number of sets in final states container. Default value for NBA (1);\n\
--edges         [number]        Number of the edges for each (not leaf and not pre-leaf) vertex in the tree.\
                                    Default value for binary tree (2);\n\
--out_file      [text]          Output file name where we will dump generated information;\n\
************************\n\
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
    bool non_optimal_only = false;
    /// \brief Input file name where we store interested automaton
    std::string in_file = "test.txt";
    /// \brief Output file name where we will dump converted automaton (if will exist)
    std::string out_file = "dump.txt";
};

/// \struct Helper to save different parts of the bfs/dfs logic
/// \note Dirty awful hack
/// \param T: supposed automaton
template<typename T>
struct emp_differences
{
    /// \brief Author info
    const char* author;
    /// \brief Program calculation mode description
    const char* user_cases_info;
    /// \brief Algorithms names that will be printed
    /// \note First NBA then NGA. @gener_cb_init.nba_algorithms + @gener_cb_initnga_algorithms size equality expected
    std::vector<std::string> algorithm_names;
    /// \brief Handle default usage case: calculation of the input automaton
    std::function<void(const options& opts)> user_case;
    /// \brief Callbacks initializer for generation
    std::function<emptiness_check::statistic::callbacks_handler<T>
            (const utils::generator::generator_opts &opts)> gener_cb_init;
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

/// \brief Print what and how we decide to generate
/// \param repetitions: number of re-running procedure for !one automaton
/// \param gen_opts: generation mode options
void print_generator_info(const automates::buchi::atm_size repetitions,
                          const utils::generator::generator_opts &opts) noexcept
{
    std::cout << "Generator will invoke " << repetitions << " for each instance of automaton (average calculation).\n"
        "\tWill be produced " << opts.states << " different generations from 10^0 to 10^" << opts.states <<
        "states per generation.\n\tEach automaton will have " << opts.sets << " sets of final states.\n\tWith maximum"
        " states / sets / edges = " << std::max(static_cast<double>(opts.states) / opts.sets / opts.edges, 1.0) <<
        " final states inside.\n\tAutomaton \"complexity\" is approximately " << opts.trees << " merged "<<
        opts.edges << "-trees.\n";
}

/// \brief Print user-friendly statistic table
/// \param stats: generated statistic
/// \param name: out file name. Otherwise console will be used
/// \param algo_headers: headers algorithms names
void print_statistic(const std::vector<emptiness_check::statistic::one_step>& stats, const std::string& name,
                     const std::vector<std::string>& algo_headers) noexcept
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

    std::vector<std::string> headers{"States", "Repetition", "Av. conversation", "Av. generation", "NGA!=NBA"};
    headers.insert(std::end(headers), algo_headers.begin(), algo_headers.end());
    t.addRow(headers);

    for (auto& stat : stats)
    {
        using namespace emptiness_check::statistic;
        auto create_word = [](std::optional<automates::buchi::atm_size> num, const call_durration& durr)
        {
            return std::to_string(durr.count()) + "us" + (num ? " (" + std::to_string(*num) + ")" : "");
        };

        std::vector<std::string> container{std::to_string(stat.states), std::to_string(stat.repetition),
                                           create_word(stat.average_conversion.first, stat.average_conversion.second),
                                           create_word(std::nullopt, stat.average_generation),
                                           std::to_string(stat.different_results)};

        for (auto& [num, durr] : stat.average_nba)
            container.emplace_back(create_word(num, durr));
        for (auto& [num, durr] : stat.average_nga)
            container.emplace_back(create_word(num, durr));

        t.addRow(container);
    }

    t.setAlignment( 0, TextTable::Alignment::RIGHT );

    *out << t;
}

/// \brief Run generation with provided parameters
/// \note Initialize callback logic
/// \param T: supposed automaton type
/// \param repetitions: number of re-running procedure for !one automaton
/// \param gen_opts: generation mode options
/// \param gen_cb: callback for the initialization of the generation logic callback
/// \return gather statistic for all stages
template<typename T>
std::vector<emptiness_check::statistic::one_step> run_generator(const automates::buchi::atm_size repetitions,
              utils::generator::generator_opts gen_opts,
              const std::function<emptiness_check::statistic::callbacks_handler<T>
                      (const utils::generator::generator_opts &opts)>& gen_cb) noexcept
{
    const auto max_opt_states = gen_opts.states;
    std::vector<emptiness_check::statistic::one_step> stats;
    stats.reserve(max_opt_states);

    for (automates::buchi::atm_size i = 0; i <= max_opt_states; ++i)
    {
        /// \note need to update gen_opts (reference inside generator callback)
        gen_opts.states = std::pow(10, i);
        auto statistic = one_step_generation(repetitions, gen_cb(gen_opts));
        statistic.states = gen_opts.states;

        stats.emplace_back(statistic);
        std::cout << "Generation of a " << statistic.states << " states completed\n";
    }

    return stats;
}

/// \brief Handle generation logic for user
/// \param T: supposed automaton type
/// \param repetitions: number of re-running procedure for !one automaton
/// \param opts: generation mode options
/// \param file_name: output file name
/// \param headers: headers algorithms names
/// \param gen_cb: callback for the initialization of the generation logic callback
template<typename T>
void handle_generator_case_call(const automates::buchi::atm_size repetitions,
                                const utils::generator::generator_opts &opts,
                                const std::string& file_name,
                                const std::vector<std::string>& headers,
                                const std::function<emptiness_check::statistic::callbacks_handler<T>
                                        (const utils::generator::generator_opts &opts)>& gen_cb) noexcept
{
    print_generator_info(repetitions, opts);

    std::vector<emptiness_check::statistic::one_step> statistics;
    // pinpoint generation time
    int durration = 0;
    {
        using namespace std::chrono;
        // starting timepoint
        auto start = high_resolution_clock::now();
        statistics = run_generator(repetitions, opts, gen_cb);
        // ending timepoint
        auto stop = high_resolution_clock::now();

        durration = duration_cast<minutes>(stop - start).count();
    }
    print_statistic(statistics, file_name, headers);

    std::cout << "Execution took " << durration << " minutes\n";
}

/// \brief Run command line with parsing parameters and invoking needed mode/calculations
/// \details Four operating modes:
///     - README or help
///     - Automaton calculation
///     - Generation calculation statistic
///     - Generation README or help
/// \param T: supposed automaton
/// \param argc: the number of command line arguments
/// \param argv: list of command-line arguments
/// \param differences: initialized dfs/bfs differences
template<typename T>
void command_line(int argc, const char *argv[], const emp_differences<T>&& differences)
{
    auto parser = CmdOpts<options>::Create({
        {"--generator", &options::generator},
        {"--help", &options::help},
        {"--nba", &options::nba},
        {"--non_optimal_only", &options::non_optimal_only},
        {"--in_file", &options::in_file},
        {"--out_file", &options::out_file},
    });
    auto opts = parser->parse(argc, argv);

    if (opts.generator)
    {
        if (opts.help)
            std::cout << GENERATE_INFO_TEXT << differences.author;
        else
        {
            using namespace utils::generator;
            auto gen_parser = CmdOpts<generator_opts>::Create({
                {"--states", &generator_opts::states},
                {"--trees", &generator_opts::trees},
                {"--sets", &generator_opts::sets},
                {"--edges", &generator_opts::edges},
            });
            auto generate_opts = gen_parser->parse(argc, argv);
            /// \note No sense take bigger. UINTMAX < 10^10
            generate_opts.states = std::min(static_cast<uint32_t>(generate_opts.states), 9u);

            handle_generator_case_call(opts.generator, generate_opts, opts.out_file,
                                       differences.algorithm_names, differences.gener_cb_init);
        }
    }
    else
    {
        if (argc == 1 || opts.help)
            std::cout << differences.user_cases_info << differences.author;
        else
            differences.user_case(opts);
    }
}

} // namespace emptiness_cmd_helper
