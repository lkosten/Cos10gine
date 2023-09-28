//
// Created by Kostya on 9/2/2023.
//

#ifndef LCOS10GINE_MOVEGENERATOR_H
#define LCOS10GINE_MOVEGENERATOR_H

#include <vector>
#include <array>

#include "BoardRepresentation/BitBoard.h"

class BitBoard;

enum MoveType : std::uint8_t {
    MoveSimple,

    CaptureSimple,
    CaptureEnPassant,
    CapturePromotion,
    PromotionSimple,

    CastlingLong,
    CastlingShort,

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
    static void GenerateWhiteKnightMoves(const BitBoard &board, std::vector<Move> *all_moves);

    static void GenerateBlackPawnMoves(const BitBoard &board, std::vector<Move> *all_moves);
    static void GenerateBlackKnightMoves(const BitBoard &board, std::vector<Move> *all_moves);

    static void GenerateBishopMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player);
    static void GenerateRookMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player);
    static void GenerateQueenMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player);
    static void GenerateKingMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player);

    static std::vector<std::uint8_t> GenerateKnightAttackPattern(std::uint8_t knight_pos);
    static std::vector<std::uint8_t> GenerateKingtAttackPattern(std::uint8_t knight_pos);
};


#endif //LCOS10GINE_MOVEGENERATOR_H
