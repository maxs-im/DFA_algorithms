##################################### libAutomaton.a #####################################
add_library(Automaton STATIC
        automaton/automates/buchi.cpp
        automaton/automates/inv_buchi.cpp
        automaton/utils/converters.cpp
        automaton/utils/representation.cpp
        automaton/utils/generator.cpp
)

target_include_directories(Automaton PUBLIC ${PROJECT_SOURCE_DIR}/include/automaton)

##################################### libEmptinessCheck.a #####################################
add_library(EmptinessCheck STATIC
    emptiness_check/dfs/nested.cpp
    emptiness_check/dfs/two_stack.cpp
    emptiness_check/bfs/emerson.cpp
    emptiness_check/bfs/modified_emerson.cpp
    emptiness_check/statistic.cpp
)

target_link_libraries(EmptinessCheck PUBLIC Automaton)
target_include_directories(EmptinessCheck PUBLIC ${PROJECT_SOURCE_DIR}/include/emptiness_check)
