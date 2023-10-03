//
// Created by Kostya on 10/4/2023.
//

#ifndef LCOS10GINE_FEN_H
#define LCOS10GINE_FEN_H


#include "../BoardRepresentation/BitBoard.h"

class BitBoard;

class FEN {
public:
    static BitBoard GetBitBoardFromFEN(const std::string &fen);

private:
    static void SetPiecePlacement(BitBoard *board, const std::string& piece_placement);
    static void SetSideToMove(BitBoard *board, const std::string& side_to_move);
    static void SetCastlingAbility(BitBoard *board, const std::string& castling);
    static void SetEnPassant(BitBoard *board, const std::string &en_passant);
    static void SetHalfMoveClock(BitBoard *board, std::string half_moves);
    static void SetFullMoveCounter(BitBoard *board, std::string full_moves);
};


#endif //LCOS10GINE_FEN_H
