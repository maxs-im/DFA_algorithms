#include <fstream>

#include "dfs/nested.hpp"
#include "dfs/two_stack.hpp"
#include "converters.hpp"

using namespace dfa;

automaton proceed_data(const char* name)
{
    std::ifstream fs;
    fs.open(name, std::fstream::in);

    assert(fs.is_open() && "Can not open file for read!");

    return construct_read(fs);
}

int main(int argc, char *argv[])
{
    using namespace emptiness_check::dfs;
    const char* name = "test.txt";

    const auto automat = proceed_data(name);

    std::cout << "Nested: " << std::boolalpha << nested::is_empty(converters::nga2nba(automat)) << "\n";
    std::cout << "Two stack: " << std::boolalpha << two_stack::is_empty(automat) << "\n";

    return 0;
}
