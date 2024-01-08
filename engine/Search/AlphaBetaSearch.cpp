//
// Created by Kostya on 1/2/2024.
//

#include <iostream>
#include <unordered_map>

#include "AlphaBetaSearch.h"
#include "TranspositionTable.h"

Move AlphaBetaSearch::GetBestMove(BitBoard board, std::unordered_map<uint64_t, size_t> prev_positions) {
    Move best_move{};
    auto eval = DFSAlphaBeta(board, 0, AlphaBeta(), best_move, prev_positions);
    std::cout << "Best move eval " << eval << '\n';
    return  best_move;
}

pos_eval AlphaBetaSearch::DFSAlphaBeta(BitBoard board, size_t depth, AlphaBetaSearch::AlphaBeta alpha_beta, Move &best_move,
                                       std::unordered_map<uint64_t, size_t> &prev_positions) {
    if (depth == 6) {
        return SimpleEvaluation::Evaluate(board);
    }

    return 0;
}
