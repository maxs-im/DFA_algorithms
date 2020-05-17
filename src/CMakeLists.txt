##################################### libAutomaton.a #####################################
add_library(Automaton STATIC
        automaton/automates/dfa.cpp
        automaton/automates/buchi.cpp
        automaton/automates/inv_buchi.cpp
        automaton/utils/converters.cpp
        automaton/utils/representation.cpp
        automaton/utils/generator.cpp
        ${PROJECT_SOURCE_DIR}/include/automaton/automates/finite_automaton.hpp
)

target_include_directories(Automaton PUBLIC ${PROJECT_SOURCE_DIR}/include/automaton)

##################################### libBinaryAlgo.a #####################################
add_library(BinaryAlgo STATIC binary_algo/binary_algo.cpp)

target_link_libraries(BinaryAlgo PUBLIC Automaton)
target_include_directories(BinaryAlgo PUBLIC ${PROJECT_SOURCE_DIR}/include/binary_algo)

##################################### libEmptinessCheck.a #####################################
add_library(EmptinessCheck STATIC
    emptiness_check/dfs/nested.cpp
    emptiness_check/dfs/two_stack.cpp
    emptiness_check/bfs/emerson.cpp
    emptiness_check/statistic.cpp
)

target_link_libraries(EmptinessCheck PUBLIC Automaton)
target_include_directories(EmptinessCheck PUBLIC ${PROJECT_SOURCE_DIR}/include/emptiness_check)
