cmake_minimum_required(VERSION 3.24)
project(lCos10gine)

set(CMAKE_CXX_STANDARD 20)

add_executable(lCos10gine main.cpp BoardRepresentation/BitBoard.cpp BoardRepresentation/BitBoard.h BoardRepresentation/PieceDefines.h MoveGenerator.cpp MoveGenerator.h)
