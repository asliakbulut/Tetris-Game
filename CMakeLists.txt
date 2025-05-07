cmake_minimum_required(VERSION 3.10)
project(TetrisCLI)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp piece.cpp game.cpp)


find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
add_executable(TetrisCLI ${SOURCE_FILES})
target_link_libraries(TetrisCLI PRIVATE sfml-system sfml-window sfml-graphics)

