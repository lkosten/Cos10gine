//
// Created by Kostya on 8/29/2023.
//

#ifndef LCOS10GINE_PIECEDEFINES_H
#define LCOS10GINE_PIECEDEFINES_H

#include <cstdint>

enum PieceType : std::uint8_t {
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,

    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing,

    PIECE_TYPE_LEN
};

enum PlayerColor : uint8_t {
    White = 0,
    Black = 1,

    PLAYER_NUMBER = 2
};

#endif //LCOS10GINE_PIECEDEFINES_H
