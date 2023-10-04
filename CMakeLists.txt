cmake_minimum_required(VERSION 3.24)
project(lCos10gine)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)
add_executable(ChessEngine ${SOURCE_FILES})

include_directories(Engine)
add_subdirectory(Engine)

target_link_libraries(ChessEngine Engine)

add_subdirectory(Tests)