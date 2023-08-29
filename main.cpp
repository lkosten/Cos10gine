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

    return 0;
}
