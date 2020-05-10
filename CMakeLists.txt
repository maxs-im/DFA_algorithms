cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
project(DFA_Algorithms LANGUAGES CXX)

set(DFA_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(apps)
