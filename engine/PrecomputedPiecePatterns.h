//
// Created by Kostya on 10/9/2023.
//

#ifndef LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H
#define LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H

#include <vector>

#include "BoardRepresentation/BitBoard.h"

class PrecomputedPiecePatterns {
public:
    static bitboard GetPawnPushPattern(PlayerColor player, squareInd pos);
    static bitboard GetPawnAttackPattern(PlayerColor player, squareInd pos);

private:
    static void GeneratePawnPrecomputePatterns();

    static std::vector<std::vector<bitboard>> kPawnPushesPattern;
    static std::vector<std::vector<bitboard>> kPawnAttacksPattern;
};


#endif //LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H
