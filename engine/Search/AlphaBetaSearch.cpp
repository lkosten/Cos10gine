//
// Created by Kostya on 1/2/2024.
//

#include <iostream>
#include <unordered_map>

#include "AlphaBetaSearch.h"

Move AlphaBetaSearch::GetBestMove(BitBoard board) {
    kStopSearch.store(false);

    Move best_move{};
    best_move = IterativeDeepening(board);
    //pos_eval eval = Negamax(board, 4, 1, -kInfinityEval, kInfinityEval, &best_move);
    return best_move;
}

void AlphaBetaSearch::ShouldStopSearch() {
    kStopSearch.store(true);
}

pos_eval AlphaBetaSearch::Negamax(const BitBoard &init_board, int depth, int ply, pos_eval alpha, pos_eval beta,
                                  Move *best_move) {
    if (kStopSearch.load()) {
        return 0;
    }

    if (depth == 0) {
        return QuiescenceSearch(init_board, alpha, beta);
    }

    size_t legal_moves = 0;
    BitBoard board = init_board;
    auto moves = MoveGenerator::GenerateMoves(board);
    for (const auto &move: moves) {
        if (board.MakeMove(move)) {
            if (kStopSearch.load()) {
                return 0;
            }

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
                    if (depth == 3)
                        std::cout << alpha << std::endl;
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
    if (kStopSearch.load()) {
        return 0;
    }

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
        if (kStopSearch.load()) {
            return 0;
        }

        if (move.type != MoveType::CaptureSimple) {
            continue;
        }

        if (board.MakeMove(move)) {
            pos_eval eval = -QuiescenceSearch(board,-beta, -alpha);

            // fail-hard beta cutoff
            if (eval >= beta) {
                return beta;
            }

            // PV node
            if (eval > alpha) {
                alpha = eval;
            }
        }
        board = init_board;
    }

    return alpha;
}

Move AlphaBetaSearch::IterativeDeepening(const BitBoard &board) {
    bool is_enough_time = true;
    int depth = 1;
    Move best_move{};
    do {
        Move cur_depth_best_move{};
        Negamax(board, depth, 1, -kInfinityEval, kInfinityEval, &cur_depth_best_move);

        if (is_enough_time /* TODO if not interrupted */) {
            best_move = cur_depth_best_move;
        }

        ++depth;
        // TODO calculate time

    } while(is_enough_time && depth <= kMaxDepth);

    return best_move;
}

std::atomic_bool AlphaBetaSearch::kStopSearch = false;
