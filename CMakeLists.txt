cmake_minimum_required(VERSION 3.10)
project(FaultyChat)

set(CMAKE_CXX_STANDARD 17)

add_executable(FaultyChat main.cpp Algorithms.cpp Algorithms.hpp Utility.cpp Utility.hpp messageProcessor.cpp messageProcessor.hpp)

add_subdirectory(win)
target_link_libraries(FaultyChat PRIVATE PDcurses)

