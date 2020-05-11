#include <fstream>
#include <iostream>

#include "dfs/nested.hpp"
#include "dfs/two_stack.hpp"
#include "utils/converters.hpp"
#include "utils/representation.hpp"

automates::buchi proceed_data(const char* name)
{
    std::ifstream fs;
    fs.open(name, std::fstream::in);

    assert(fs.is_open() && "Can not open file for read!");

    return utils::representation::construct_read(fs, true);
}

int main(int argc, char *argv[])
{
    using namespace emptiness_check::dfs;
    const char* name = "test_ec.txt";

    const auto automat = proceed_data(name);
    const auto nba_automat = automat.is_generalized() ?
            utils::converters::nga2nba(automat) :
            automat;

    std::cout << "Nested: " << std::boolalpha << nested::is_empty(nba_automat) << "\n";
    std::cout << "Two stack on NBA: " << std::boolalpha << two_stack::is_empty(nba_automat) << "\n";
    if (automat.is_generalized())
    {
        std::cout << "Two stack on NGA: " << std::boolalpha << two_stack::is_empty(automat) << "\n";
    }

    std::cout << nba_automat << automat;

    return 0;
}
