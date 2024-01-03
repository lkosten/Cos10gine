//
// Created by Kostya on 1/2/2024.
//

#include "AlphaBetaSearch.h"

Move AlphaBetaSearch::GetBestMove(BitBoard board) {
    Move best_move{};
    DFSAlphaBeta(board, 0, AlphaBeta(), best_move);
    return  best_move;
}

pos_eval AlphaBetaSearch::DFSAlphaBeta(BitBoard board, size_t depth, AlphaBetaSearch::AlphaBeta alpha_beta, Move &best_move) {
    if (depth == 6) {
        return SimpleEvaluation::Evaluate(board);
    }

    pos_eval evaluation;
    if (board.GetPlayerToMove() == PlayerColor::White) {
        // Maximizing player
        evaluation = -1'000'000'0;

        auto moves = MoveGenerator::GenerateMoves(board);
        for (const auto move : moves) {
            pos_eval cur_eval;
            if (!board.MakeMove(move)) {
                cur_eval = -1'000'000'0;
            }
            else {
                cur_eval = DFSAlphaBeta(board, depth + 1, alpha_beta, best_move);
            }
            board.UnMakeMove(move);

            if (cur_eval > evaluation) {
                evaluation = cur_eval;
                if (depth == 0) {
                    best_move = move;
                }
            }

            alpha_beta.alpha = std::max(alpha_beta.alpha, evaluation);
            if (alpha_beta.beta <= alpha_beta.alpha) {
                break;
            }
        }
    }
    else {
        // Minimizing player
        evaluation = 1'000'000'0;

        auto moves = MoveGenerator::GenerateMoves(board);
        for (const auto move : moves) {
            pos_eval cur_eval;
            if (!board.MakeMove(move)) {
                cur_eval = 1'000'000'0;
            }
            else {
                cur_eval = DFSAlphaBeta(board, depth + 1, alpha_beta, best_move);
            }
            board.UnMakeMove(move);

            if (cur_eval < evaluation) {
                evaluation = cur_eval;
                if (depth == 0) {
                    best_move = move;
                }
            }

            alpha_beta.beta = std::min(alpha_beta.beta, evaluation);
            if (alpha_beta.beta <= alpha_beta.alpha) {
                break;
            }
        }
    }

    return evaluation;
}
