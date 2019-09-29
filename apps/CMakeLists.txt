add_executable(lab1 lab1.cpp)
target_compile_features(lab1 PRIVATE cxx_std_17)

target_link_libraries(lab1 PRIVATE binary_algo)
