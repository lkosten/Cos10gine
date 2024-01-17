//
// Created by Kostya on 1/12/2024.
//

#include <string>
#include <mutex>

#include "GrandMaster.h"
#include "Search/AlphaBetaSearch.h"

void GrandMaster::CreateNewGame(const std::string& fen) {
    std::lock_guard lock(board_protection);

    if (fen.empty()) {
        game = ChessGame();
    }
    else {
        game = ChessGame(fen);
    }
}

BitBoard GrandMaster::GetCurrentBoard() const {
    std::lock_guard lock(board_protection);

    return game.GetLastBoard();
}

bool GrandMaster::TryMakeMove(const Move &move) {
    std::lock_guard lock(board_protection);

    return game.TryMakeMove(move);
}

Move GrandMaster::GetBestMove() const {
    return AlphaBetaSearch::GetBestMove(GetCurrentBoard());
}
