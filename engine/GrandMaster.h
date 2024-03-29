//
// Created by Kostya on 1/12/2024.
//

#ifndef LCOS10GINE_GRANDMASTER_H
#define LCOS10GINE_GRANDMASTER_H

#include <mutex>
#include "ChessGame.h"

struct SearchLimits;

class GrandMaster {
public:
    GrandMaster() = default;

    void CreateNewGame(const std::string& fen = "");

    bool TryMakeMove(const Move &move);

    Move GetBestMove(SearchLimits limits) const;
    BitBoard GetCurrentBoard() const;

private:
    ChessGame game;
    mutable std::mutex board_protection;
};


#endif //LCOS10GINE_GRANDMASTER_H
