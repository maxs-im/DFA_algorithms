set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DFA_BINARY_DIR})

##################################### binalgo #####################################
add_executable(binalgo bin_algo.cpp)
target_link_libraries(binalgo PRIVATE BinaryAlgo)

file(INSTALL test.txt DESTINATION ${DFA_BINARY_DIR})

##################################### course #####################################
add_executable(emptiness emptiness.cpp)
target_link_libraries(emptiness PRIVATE EmptinessCheck)
