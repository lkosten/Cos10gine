//
// Created by Kostya on 10/9/2023.
//

#ifndef LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H
#define LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H

#include <vector>

#include "BoardRepresentation/BitBoard.h"
#include "MagicBitboards.h"

class PrecomputedPiecePatterns {
public:
    static bitboard GetPawnPushPattern(PlayerColor player, squareInd pos);
    static bitboard GetPawnAttackPattern(PlayerColor player, squareInd pos);
    static bitboard GetKnightAttackPattern(squareInd pos);
    static bitboard GetKingAttackPattern(squareInd pos);

    static bitboard GetBishopAttackPattern(squareInd pos, bitboard blockers);
    static bitboard GetRookAttackPattern(squareInd pos, bitboard blockers);
    static bitboard GetQueenAttackPattern(squareInd pos, bitboard blockers);

private:
    static std::vector<std::vector<bitboard>> GeneratePawnPushesPrecomputePatterns();
    static std::vector<std::vector<bitboard>> GeneratePawnAttacksPrecomputePatterns();
    static std::vector<bitboard> GenerateKnightPrecomputePatterns();
    static std::vector<bitboard> GenerateKingPrecomputePatterns();

    static std::vector<std::vector<bitboard>> kPawnPushesPattern;
    static std::vector<std::vector<bitboard>> kPawnAttacksPattern;
    static std::vector<bitboard> kKnightAttacksPattern;
    static std::vector<bitboard> kKingAttacksPattern;

    MagicBitboards magic_bitboards;
};


#endif //LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H
