cmake_minimum_required(VERSION 3.14)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)
enable_testing()

add_executable(untitled1 task.cpp)

add_executable(tests test.cpp)
target_link_libraries(tests gtest gtest_main)

target_compile_definitions(tests PRIVATE TESTING)

add_test(NAME AllTests COMMAND tests)
