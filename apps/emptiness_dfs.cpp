/// \author Maksym Halchenko
/// \date 2020

#include <fstream>
#include <iostream>

#include "dfs/nested.hpp"
#include "dfs/two_stack.hpp"
#include "utils/converters.hpp"
#include "utils/representation.hpp"

#include "LightweightParsingCMD.hpp"

/// \brief Program description
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
emptiness_dfs usage:\n\
\n\
--help                  Show info about this binary (programm);\n\
--generator             TODO:;\n\
--nba                   Works only with NBA (converts NGA if needed);\n\
--nested                Invokes Nested algorithm. We use Two-stack by default (because optimal);\n\
--in_file               Input file name where we store interested automaton;\n\
--out_file              Output file name where we will dump converted automaton (if will exist);\n\
************************\n\
Return true or false for selected algorithm\n\
\n\
Author: Maksym Halchenko\n\
    Github: @maxs-im\n\
Year: 2020\n\
";

/// \struct options It store command line arguments
struct options
{
    /// \brief Show info about this binary (programm)
    bool help = false;
    /// \brief TODO:
    bool generator = false;
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
automates::buchi proceed_data(const char* name) noexcept
{
    std::istream *in = &std::cin;
    std::ifstream fs;
    if (name)
    {
        fs.open(name, std::fstream::in);
        if (!fs.is_open())
            std::cerr << "Failed to read from " << name << " file!\n";
        else
            in = &fs;
    }

    auto automat = utils::representation::construct_read(*in, true);
    std::cout << "Successfully read from " << name << " file\n";
    return automat;
}

/// \brief Help to conavert automaton to the NBA and dump logs if needed
/// \param automaton: automaton for the conversation
/// \param name: output file name. Dumping will be ignored on incorrect file
/// \return converted automaton
automates::buchi proceed_conversion(const automates::buchi& automaton, const char* name) noexcept
{
    automates::buchi converted_automat = utils::converters::nga2nba(automaton);
    std::cout << "Successfully converted to NGA\n";

    if (name)
    {
        std::ofstream fs(name, std::fstream::out);
        if (!fs.is_open())
            std::cerr << "Failed to dump to " << name << " file!\n";
        else
        {
            fs << converted_automat;
            std::cout << "Successfully dumped into the " << name << " file\n";
        }
    }

    return converted_automat;
}

int main(int argc, const char *argv[])
{
    auto parser = CmdOpts<options>::Create({
        {"--help", &options::help},
        {"--generator", &options::generator},
        {"--nba", &options::nba},
        {"--nested", &options::nested},
        {"--in_file", &options::in_file},
        {"--out_file", &options::out_file},
    });

    auto opts = parser->parse(argc, argv);
    if (argc == 1 || opts.help)
    {
        std::cout << INFO_TEXT;
    }
    else if (opts.generator)
    {

    }
    else
    {
        auto automaton = proceed_data(opts.in_file.c_str());

        // need to convert in case of Nested algorithm for NGA
        auto worked_automaton = (opts.nba || opts.nested) && automaton.is_generalized() ?
                proceed_conversion(automaton, opts.out_file.c_str()) :
                automaton;

        using namespace emptiness_check::dfs;
        std::cout << std::boolalpha << "...\n";
        if (opts.nested)
            std::cout << "Nested: " << nested::is_empty(worked_automaton) << "\n";
        else
            std::cout << "Two-stack (" << (worked_automaton.is_generalized() ? "NGA" : "NBA") << "): " <<
                         two_stack::is_empty(worked_automaton) << "\n";
    }

    return 0;
}
