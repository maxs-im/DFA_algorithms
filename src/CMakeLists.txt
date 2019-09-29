add_library(automaton_library automaton.cpp)
add_library(binary_algo binary_algo.cpp automaton.cpp)

target_link_libraries(binary_algo PUBLIC automaton_library)

target_include_directories(automaton_library PUBLIC ../include/utils)
target_include_directories(binary_algo PUBLIC ../include)

target_compile_features(automaton_library PUBLIC cxx_std_11)
target_compile_features(binary_algo PUBLIC cxx_std_11)
