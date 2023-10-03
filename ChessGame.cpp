//
// Created by Kostya on 9/29/2023.
//

#include <iostream>
#include "ChessGame.h"

ChessGame::ChessGame() : f_boards() {
    f_boards.push_back(BitBoard::GetStartBoard());
}

ChessGame::ChessGame(const std::string &fen) : f_boards() {
    f_boards.push_back(FEN::GetBitBoardFromFEN(fen));
}

bool ChessGame::TryMakeMove(const Move &move) {
    auto last_board = f_boards.back();

    if (last_board.MakeMove(move)) {
        f_boards.push_back(last_board);
        return true;
    }
    else {
        //last_board.DebugDraw(std::cout);
        return false;
    }
}

void ChessGame::UnMakeMove() {
    f_boards.pop_back();
}

BitBoard ChessGame::GetLastBoard() const {
    return f_boards.back();
}
