//
// Created by Kostya on 9/29/2023.
//

#ifndef LCOS10GINE_CHESSGAME_H
#define LCOS10GINE_CHESSGAME_H

#include <stack>
#include <unordered_map>
#include "BoardRepresentation/BitBoard.h"

class ChessGame {
public:
    ChessGame();
    explicit ChessGame(const std::string &fen);

    bool TryMakeMove(const Move &move);
    void UnMakeMove();

    BitBoard GetLastBoard() const;
    std::unordered_map<uint64_t, size_t> GetPositionsCount() const;

private:
    std::deque<BitBoard> f_boards;
    std::unordered_map<uint64_t, size_t> f_positions_count;
};


#endif //LCOS10GINE_CHESSGAME_H
