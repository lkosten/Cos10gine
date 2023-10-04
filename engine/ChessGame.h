//
// Created by Kostya on 9/29/2023.
//

#ifndef LCOS10GINE_CHESSGAME_H
#define LCOS10GINE_CHESSGAME_H

#include <stack>
#include "BoardRepresentation/BitBoard.h"

class ChessGame {
public:
    ChessGame();
    ChessGame(const std::string &fen);

    bool TryMakeMove(const Move &move);
    void UnMakeMove();

    BitBoard GetLastBoard() const;

private:
    std::deque<BitBoard> f_boards;
};


#endif //LCOS10GINE_CHESSGAME_H
