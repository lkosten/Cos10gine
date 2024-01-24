//
// Created by Kostya on 1/2/2024.
//

#include "SimpleEvaluation.h"
#include "BoardRayIterator.h"

pos_eval SimpleEvaluation::Evaluate(const BitBoard &board) {
    pos_eval eval = 0;

    bool is_endgame = board.GetPiecePositions(PieceType::WhiteQueen) == 0 &&
                      board.GetPiecePositions(PieceType::BlackQueen) == 0;

    for (std::uint8_t piece = 0; piece < PieceType::PIECE_TYPE_LEN; ++piece) {
        bitboard all_piece_bb = board.GetPiecePositions(static_cast<PieceType>(piece));

        auto pieces = BoardRayIterator::Bitscan(all_piece_bb);
        for (const auto piece_bb : pieces) {
            eval += kPieceValues[piece];

            squareInd piece_pos = BoardRayIterator::MS1BInd(piece_bb);

            if (piece < PieceType::BlackPawn) {
                squareInd file = piece_pos & 7;
                squareInd rank = piece_pos >> 3;
                rank = 7 - rank;
                piece_pos = 8 * rank + file;

                eval += kPieceSquareTables[piece + (is_endgame && piece == WhiteKing ? 1 : 0)][piece_pos];
            }
            else {
                eval -= kPieceSquareTables[piece - 6 + (is_endgame && piece == BlackKing ? 1 : 0)][piece_pos];
            }
        }
    }
    return eval;
}
