//
// Created by Kostya on 1/2/2024.
//

#ifndef LCOS10GINE_ALPHABETASEARCH_H
#define LCOS10GINE_ALPHABETASEARCH_H

#include <atomic>
#include "BoardRepresentation/BitBoard.h"
#include "Evaluation/SimpleEvaluation.h"
#include "Interfaces/UCI.h"

class AlphaBetaSearch {
public:
    static Move GetBestMove(const BitBoard& board, SearchLimits limits);

    static void ShouldStopSearch();

private:
    static pos_eval Negamax(const BitBoard &board, int depth, int ply, pos_eval alpha, pos_eval beta, Move *best_move);
    static pos_eval QuiescenceSearch(const BitBoard &board, pos_eval alpha, pos_eval beta);
    static Move IterativeDeepening(const BitBoard &board, SearchLimits limits);
    static uint32_t CalculateTimeForMove(const BitBoard &board, SearchLimits limits);

    static const pos_eval kMateEval = 100'000'000;
    static const pos_eval kInfinityEval = 1'000'000'000;
    static const uint8_t kMaxDepth = 255;

    static std::atomic_bool kStopSearch;
};


#endif //LCOS10GINE_ALPHABETASEARCH_H
