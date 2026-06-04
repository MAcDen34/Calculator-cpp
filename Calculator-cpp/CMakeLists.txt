cmake_minimum_required(VERSION 3.15)
project(CalculatorTests)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Source library
add_library(calc_lib
    src/calculator.cpp
    src/parser.cpp
)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(tests tests/test_calculator.cpp)
target_link_libraries(tests
    calc_lib
    GTest::GTest
    GTest::Main
)
