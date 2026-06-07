cmake_minimum_required(VERSION 3.15)
project(Calculator)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

# Source library
add_library(calc_lib
    src/calculator.cpp
    src/parser.cpp
    src/history.cpp
    src/history_writer.cpp
)

# Google Test
find_package(GTest REQUIRED)
add_executable(tests tests/test_calculator.cpp)
target_link_libraries(tests calc_lib GTest::GTest GTest::Main)

# FTXUI
include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# CLI executable
add_executable(calculator cli/main.cpp)
target_link_libraries(calculator
    calc_lib
    ftxui::screen
    ftxui::dom
    ftxui::component
)

# Qt6 GUI (optional - skipped in Docker)
find_package(Qt6 QUIET COMPONENTS Widgets Charts)
if(Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    add_executable(calculator_gui gui/main.cpp gui/mainwindow.cpp)
    target_link_libraries(calculator_gui calc_lib Qt6::Widgets Qt6::Charts)
endif()
