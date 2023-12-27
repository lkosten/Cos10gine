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
    static bitboard GetKnightAttackPattern(squareInd pos);
    static bitboard GetKingAttackPattern(squareInd pos);

private:
    static std::vector<std::vector<bitboard>> GeneratePawnPushesPrecomputePatterns();
    static std::vector<std::vector<bitboard>> GeneratePawnAttacksPrecomputePatterns();
    static std::vector<bitboard> GenerateKnightPrecomputePatterns();
    static std::vector<bitboard> GenerateKingPrecomputePatterns();

    static std::vector<std::vector<bitboard>> kPawnPushesPattern;
    static std::vector<std::vector<bitboard>> kPawnAttacksPattern;
    static std::vector<bitboard> kKnightAttacksPattern;
    static std::vector<bitboard> kKingAttacksPattern;

    static std::vector<uint64_t> kBishopMagicNumbers;
    static std::vector<size_t> kBishopMagicNumberFreedomDegrees;

    static std::vector<uint64_t> kRookMagicNumbers;
    static std::vector<size_t> kRookMagicNumberFreedomDegrees;
};


#endif //LCOS10GINE_PRECOMPUTEDPIECEPATTERNS_H
