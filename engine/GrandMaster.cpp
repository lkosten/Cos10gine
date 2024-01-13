//
// Created by Kostya on 1/12/2024.
//

#include <string>

#include "GrandMaster.h"
#include "Search/AlphaBetaSearch.h"

void GrandMaster::CreateNewGame(const std::string& fen) {
    if (fen.empty()) {
        game = ChessGame();
    }
    else {
        game = ChessGame(fen);
    }
}

BitBoard GrandMaster::GetCurrentBoard() const {
    return game.GetLastBoard();
}

bool GrandMaster::TryMakeMove(const Move &move) {
    return game.TryMakeMove(move);
}

Move GrandMaster::GetBestMove() const {
    return AlphaBetaSearch::GetBestMove(game.GetLastBoard());
}
