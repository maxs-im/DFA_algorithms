cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)
project(DFA_Labs LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(apps)

install(TARGETS lab1 DESTINATION bin)
