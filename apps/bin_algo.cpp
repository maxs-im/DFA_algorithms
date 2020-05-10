#include <iostream>
#include <fstream>

#include "binary_algo.hpp"

using namespace automates;

dfa proceed_data(const char* name, binary_algo::OPERATOR op, bool pair_print)
{
    std::ifstream fs;
    fs.open(name, std::fstream::in);
    
    assert(fs.is_open() && "Can not open file for read!");
    
    auto a(construct_read(fs)), b(construct_read(fs));

    fs.close();
    return binary_algo::do_act(a, b, op, pair_print);
}

int main(int argc, char *argv[]) 
{
    using namespace binary_algo;
    const char* name = "test.txt";
    OPERATOR op = OPERATOR::DIFFERENCE;
    bool pair_print = true;

    if (argc > 1)
    {
        op = static_cast<OPERATOR>(std::stoi(argv[1]) % 3);
    }
    if (argc > 2)
    {
        name = argv[2];
    }
    if (argc > 3) 
    {
        pair_print = true;
    }
    
    auto c = proceed_data(name, op, pair_print);
    c.print(std::cout); 

    return 0;
}