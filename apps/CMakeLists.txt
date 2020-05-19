set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DFA_BINARY_DIR})

file(INSTALL
        test.txt
    DESTINATION
        ${DFA_BINARY_DIR}
)

##################################### emptiness_dfs #####################################
add_executable(emptiness_dfs emptiness_dfs.cpp include/)
target_link_libraries(emptiness_dfs PRIVATE EmptinessCheck)

##################################### emptiness_bfs #####################################
add_executable(emptiness_bfs emptiness_bfs.cpp include/)
target_link_libraries(emptiness_bfs PRIVATE EmptinessCheck)
