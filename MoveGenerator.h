//
// Created by Kostya on 9/2/2023.
//

#ifndef LCOS10GINE_MOVEGENERATOR_H
#define LCOS10GINE_MOVEGENERATOR_H

#include <vector>

#include "BoardRepresentation/BitBoard.h"

class BitBoard;

enum MoveType : std::uint8_t {
    MoveSimple,

    CaptureSimple,
    CaptureEnPassant,
    CapturePromotion,
    PromotionSimple,

    CastlingLeft,
    CastlingRight,

    MOVE_TYPE_LEN
};

struct Move {
    Move() = default;

    std::uint8_t source_square;
    std::uint8_t target_square;

    MoveType type;

    PieceType source_piece;
    PieceType target_piece;

    PieceType promotion_piece;
};

class MoveGenerator {
public:

    static std::vector<Move> GenerateMoves(const BitBoard& board);

private:

    static bitboard GenerateOccupiedPositions(const BitBoard &board);
    static bitboard GenerateBlackOccupiedPositions(const BitBoard &board);
    static bitboard GenerateWhiteOccupiedPositions(const BitBoard &board);

    static void GenerateWhitePawnMoves(const BitBoard &board, std::vector<Move> *all_moves);
};


#endif //LCOS10GINE_MOVEGENERATOR_H
