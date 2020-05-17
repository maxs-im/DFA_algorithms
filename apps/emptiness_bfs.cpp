#include <fstream>
#include <iostream>

#include "bfs/emerson.hpp"
#include "utils/converters.hpp"
#include "utils/representation.hpp"

automates::buchi proceed_data(const char* name)
{
    std::ifstream fs;
    fs.open(name, std::fstream::in);

    assert(fs.is_open() && "Can not open file for read!");

    return utils::representation::construct_read(fs);
}

int main(int argc, char *argv[])
{
    using namespace emptiness_check::bfs;
    const char* name = "test_bfs.txt";

    auto automat = proceed_data(name);
    const auto nba_automat = automat.is_generalized() ?
                             utils::converters::nga2nba(automat) :
                             automat;

    // TODO: Example of use
    automates::inv_buchi inv_automat(std::move(automat));

    std::cout << "Emerson: " << std::boolalpha << emerson::is_empty(inv_automat) << "\n";
    if (inv_automat.is_generalized())
    {
        std::cout << "Two stack on NGA: " << std::boolalpha << emerson::is_empty(inv_automat) << "\n";
    }

    if (nba_automat)
        std::cout << *nba_automat;

    return 0;
}
