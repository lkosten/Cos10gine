//
// Created by Kostya on 1/2/2024.
//

#ifndef LCOS10GINE_ALPHABETASEARCH_H
#define LCOS10GINE_ALPHABETASEARCH_H

#include "BoardRepresentation/BitBoard.h"
#include "Evaluation/SimpleEvaluation.h"

class AlphaBetaSearch {
public:
    static Move GetBestMove(BitBoard board);

private:
    static pos_eval Negamax(const BitBoard &board, int depth, int ply, pos_eval alpha, pos_eval beta, Move *best_move);

    static const pos_eval kMateEval = 100'000'000;
    static const pos_eval kInfinityEval = 1'000'000'000;
};


#endif //LCOS10GINE_ALPHABETASEARCH_H
