//
// Created by Kostya on 9/2/2023.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::GenerateLegalMoves(const BitBoard& board) {
    std::vector<Move> all_moves;

    bitboard occupied_positions = GenerateOccupiedPositions(board);

    return all_moves;
}

bitboard MoveGenerator::GenerateOccupiedPositions(const BitBoard &board) {
    bitboard occupied = 0;
    for (size_t ind = 0; ind < PieceType::PIECE_TYPE_LEN; ++ind) {
        occupied |= board.GetPiecePositions(static_cast<PieceType>(ind));
    }

    return occupied;
}
