#include <iostream>
#include <fstream>
#include <windows.h>

#include "BoardRepresentation/BitBoard.h"

#pragma execution_character_set( "utf-8" )

int main() {
    SetConsoleOutputCP( 65001 );

    BitBoard bb{};
    bb = BitBoard::GetStartBoard();
    bb.DebugDraw(std::cout);
    std::cout << std::endl;
    std::cout << std::endl;

    auto moves = MoveGenerator::GenerateMoves(bb);

    for (const auto &move : moves) {
        bb.MakeMove(move);

        bb.DebugDraw(std::cout);
        std::cout << std::endl;
        bb.UnMakeMove(move);
    }


    return 0;
}
