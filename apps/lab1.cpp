// Lab1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <string> 
#include <tuple>
#include <cassert>

#include <binary_algo.hpp>

using namespace dfa;

automaton proceed_data(char* name, binary_algo::OPERATOR op)
{
    std::ifstream fs;
    fs.open(name, std::fstream::in);
    
    assert(fs.is_open() && "Can not open file for read!");
    
    auto a(construct_read(fs)), b(construct_read(fs));

    fs.close();
    return binary_algo::do_act(a, b, op);
}

int main(int argc, char *argv[]) 
{
    using namespace binary_algo;
    char* name = "test.txt";
    OPERATOR op = OPERATOR::DIFFERENCE;

    if (argc > 1)
    {
        op = static_cast<OPERATOR>(std::stoi(argv[1]) % 3);
    }
    if (argc > 2)
    {
        name = argv[2];
    }
    
    auto c = proceed_data(name, op);
    c.print(std::cout); 

    return 0;
}