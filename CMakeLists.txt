cmake_minimum_required(VERSION 3.24)
project(lCos10gine)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)
add_executable(ChessEngine ${SOURCE_FILES})

include_directories(engine)
add_subdirectory(engine)

target_link_libraries(ChessEngine ChessEngine_lib)

add_subdirectory(tests)
