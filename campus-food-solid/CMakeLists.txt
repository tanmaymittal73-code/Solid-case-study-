cmake_minimum_required(VERSION 3.10)
project(CampusFoodSolid CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include extensions)

add_executable(campus_food src/main.cpp)
add_executable(extension_demo examples/extension_demo.cpp)
add_executable(run_tests tests/test_main.cpp)

enable_testing()
add_test(NAME all_tests COMMAND run_tests)
