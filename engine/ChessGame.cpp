//
// Created by Kostya on 9/29/2023.
//

#include <iostream>
#include "ChessGame.h"

ChessGame::ChessGame() : f_boards(), f_positions_count() {
    f_boards.push_back(BitBoard::GetStartBoard());
    f_positions_count[f_boards.back().GetZobristHash()]++;
}

ChessGame::ChessGame(const std::string &fen) : f_boards(), f_positions_count() {
    f_boards.push_back(FEN::GetBitBoardFromFEN(fen));
    f_positions_count[f_boards.back().GetZobristHash()]++;
}

bool ChessGame::TryMakeMove(const Move &move) {
    auto last_board = f_boards.back();

    if (last_board.MakeMove(move)) {
        f_boards.push_back(last_board);
        f_positions_count[f_boards.back().GetZobristHash()]++;
        return true;
    }
    else {
        //last_board.DebugDraw(std::cout);
        return false;
    }
}

void ChessGame::UnMakeMove() {
    f_positions_count[f_boards.back().GetZobristHash()]--;
    f_boards.pop_back();
}

BitBoard ChessGame::GetLastBoard() const {
    return f_boards.back();
}

std::unordered_map<uint64_t, size_t> ChessGame::GetPositionsCount() const {
    return f_positions_count;
}
