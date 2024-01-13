//
// Created by Kostya on 1/2/2024.
//

#include <iostream>
#include <unordered_map>

#include "AlphaBetaSearch.h"

Move AlphaBetaSearch::GetBestMove(BitBoard board) {
    Move best_move{};
    pos_eval eval = Negamax(board, 6, 1, -kInfinityEval, kInfinityEval, &best_move);
    return best_move;
}

pos_eval AlphaBetaSearch::Negamax(const BitBoard &init_board, int depth, int ply, pos_eval alpha, pos_eval beta,
                                  Move *best_move) {
    if (depth == 0) {
        return QuiescenceSearch(init_board, alpha, beta);
    }

    size_t legal_moves = 0;
    BitBoard board = init_board;
    auto moves = MoveGenerator::GenerateMoves(board);
    for (const auto &move: moves) {
        if (board.MakeMove(move)) {
            ++legal_moves;

            pos_eval eval = -Negamax(board, depth - 1, ply + 1, -beta, -alpha, best_move);

            // fail-hard beta cutoff
            if (eval >= beta) {
                return beta;
            }

            // PV node
            if (eval > alpha) {
                alpha = eval;

                if (ply == 1) {
                    *best_move = move;
                }
            }
        }

        board = init_board;
    }

    // if old_alpha != alpha new best move

    if (legal_moves == 0) {
        if (MoveGenerator::IsKingInCheck(board)) {
            return -kMateEval + ply;
        } else {
            // stalemate
            return 0;
        }
    }

    return alpha;
}

pos_eval AlphaBetaSearch::QuiescenceSearch(const BitBoard &init_board, pos_eval alpha, pos_eval beta) {
    pos_eval stand_pat = SimpleEvaluation::Evaluate(init_board) *
                    (init_board.GetPlayerToMove() == PlayerColor::White ? 1 : -1);

    // fail-hard beta cutoff
    if (stand_pat >= beta) {
        return beta;
    }

    if (stand_pat > alpha) {
        alpha = stand_pat;
    }

    BitBoard board = init_board;
    auto moves = MoveGenerator::GenerateMoves(board);
    for (const auto &move: moves) {
        if (move.type != MoveType::CaptureSimple && move.type != MoveType::CapturePromotion &&
            move.type != MoveType::CaptureEnPassant) {
            continue;
        }

        if (board.MakeMove(move)) {
            pos_eval eval = -QuiescenceSearch(board,-beta, -alpha);
            board = init_board;

            // fail-hard beta cutoff
            if (eval >= beta) {
                return beta;
            }

            // PV node
            if (eval > alpha) {
                alpha = eval;
            }
        }
    }

    return alpha;
}
