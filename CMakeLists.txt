cmake_minimum_required(VERSION 3.0)
project(LongNumber)
find_package(GTest)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(src)
add_subdirectory(googletest)
include(GoogleTest)
add_executable(tests
        my_tests/bool_test.cpp
        my_tests/binary_mult_test.cpp
        my_tests/binary_plus_test.cpp
        my_tests/binary_dev_test.cpp
        my_tests/binary_sub_test.cpp)
enable_testing()
add_executable(pi main.cpp)
target_link_libraries(pi LongNumber)
target_link_libraries(tests LongNumber gtest gtest_main)