set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DFA_BINARY_DIR})

##################################### binalgo #####################################
add_executable(binalgo bin_algo.cpp)
target_link_libraries(binalgo PRIVATE BinaryAlgo)

file(INSTALL
        test.txt
        test_dfs.txt
        test_bfs.txt
    DESTINATION
        ${DFA_BINARY_DIR}
)

##################################### emptiness_dfs #####################################
add_executable(emptiness_dfs emptiness_dfs.cpp LightweightParsingCMD.hpp)
target_link_libraries(emptiness_dfs PRIVATE EmptinessCheck)

##################################### emptiness_bfs #####################################
add_executable(emptiness_bfs emptiness_bfs.cpp LightweightParsingCMD.hpp)
target_link_libraries(emptiness_bfs PRIVATE EmptinessCheck)
