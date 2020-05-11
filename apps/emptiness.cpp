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

    std::cout << "Nested: " << std::boolalpha << nested::is_empty(utils::converters::nga2nba(automat)) << "\n";
    std::cout << "Two stack: " << std::boolalpha << two_stack::is_empty(automat) << "\n";

    std::cout << automat;

    return 0;
}
