//
// Created by Kostya on 1/2/2024.
//

#ifndef LCOS10GINE_ALPHABETASEARCH_H
#define LCOS10GINE_ALPHABETASEARCH_H

#include "BoardRepresentation/BitBoard.h"
#include "Evaluation/SimpleEvaluation.h"

class AlphaBetaSearch {
public:
    static Move GetBestMove(BitBoard board, std::unordered_map<uint64_t, size_t> prev_positions = std::unordered_map<uint64_t, size_t>());

private:
    struct AlphaBeta {
        AlphaBeta() : alpha(-1'000'000'0), beta(1'000'000'0) {}

        pos_eval alpha;
        pos_eval beta;
    };

    static pos_eval DFSAlphaBeta(BitBoard board, size_t depth, AlphaBeta alpha_beta, Move &best_move, std::unordered_map<uint64_t, size_t> &prev_positions);
};


#endif //LCOS10GINE_ALPHABETASEARCH_H
