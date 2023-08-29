//
// Created by Kostya on 8/29/2023.
//

#ifndef LCOS10GINE_BITBOARD_H
#define LCOS10GINE_BITBOARD_H

#include <cstdint>

#include "PieceDefines.h"

typedef std::uint64_t bitboard;

/*
 * LSF mapping (Least Significant File)
 * LSF squareIndex = 8 * rankIndex + fileIndex
 * FileIndex   = squareIndex modulo 8  = squareIndex & 7
 * RankIndex   = squareIndex div    8  = squareIndex >> 3
 *
 * Little-endian mapping
 * a < h, 0 < 7
 *
 *   a1, b1, c1, d1, e1, f1, g1, h1,
 *   a2, b2, c2, d2, e2, f2, g2, h2,
 *   a3, b3, c3, d3, e3, f3, g3, h3,
 *   a4, b4, c4, d4, e4, f4, g4, h4,
 *   a5, b5, c5, d5, e5, f5, g5, h5,
 *   a6, b6, c6, d6, e6, f6, g6, h6,
 *   a7, b7, c7, d7, e7, f7, g7, h7,
 *   a8, b8, c8, d8, e8, f8, g8, h8
 *
 * a-file             0x0101010101010101
 * h-file             0x8080808080808080
 * 1st rank           0x00000000000000FF
 * 8th rank           0xFF00000000000000
 * a1-h8 diagonal     0x8040201008040201
 * h1-a8 antidiagonal 0x0102040810204080
 * light squares      0x55AA55AA55AA55AA
 * dark squares       0xAA55AA55AA55AA55
 */

class BitBoard {
public:

    static BitBoard GetStartBoard();

    bitboard GetPiecePositions(PieceType piece);

private:
    bitboard f_board[PieceType::PIECE_TYPE_LEN];
};


#endif //LCOS10GINE_BITBOARD_H
