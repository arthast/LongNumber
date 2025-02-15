cmake_minimum_required(VERSION 3.28)
project(LongNumber)
find_package(GTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(googleTests)
add_subdirectory(LongNumber)
include(GoogleTest)

add_executable(tests
        my_tests/divide_tests.cpp
        my_tests/mult_tests.cpp
        my_tests/plus_tests.cpp
        my_tests/sub_tests.cpp
        my_tests/comp.cpp)
enable_testing()
add_executable(main main.cpp)
target_link_libraries(main LongNumber)
target_link_libraries(tests LongNumber gtest gtest_main)