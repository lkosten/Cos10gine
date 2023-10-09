//
// Created by Kostya on 10/9/2023.
//

#include "PrecomputedPiecePatterns.h"

std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::kPawnPushesPattern;
std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::kPawnAttacksPattern;

void PrecomputedPiecePatterns::GeneratePawnPrecomputePatterns() {
    if (kPawnPushesPattern.size() == 2) {
        return;
    }

    kPawnPushesPattern.assign(PLAYER_NUMBER, std::vector<bitboard>(64, 0));
    kPawnAttacksPattern.assign(PLAYER_NUMBER, std::vector<bitboard>(64, 0));

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);

        bitboard pushes_bb = 0;
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k8RankBitboard) == 0) {
            pushes_bb |= (square_bb << 8);
        }
        if ((square_bb & BitBoard::k2RankBitboard) != 0) {
            pushes_bb |= (square_bb << 16);
        }

        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb << 7);
        }
        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb << 9);
        }

        kPawnPushesPattern[PlayerColor::White][square] = pushes_bb;
        kPawnAttacksPattern[PlayerColor::White][square] = attacks_bb;
    }

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);

        bitboard pushes_bb = 0;
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k1RankBitboard) == 0) {
            pushes_bb |= (square_bb >> 8);
        }
        if ((square_bb & BitBoard::k7RankBitboard) != 0) {
            pushes_bb |= (square_bb >> 16);
        }

        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 9);
        }
        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 7);
        }

        kPawnPushesPattern[PlayerColor::Black][square] = pushes_bb;
        kPawnAttacksPattern[PlayerColor::Black][square] = attacks_bb;
    }
}

bitboard PrecomputedPiecePatterns::GetPawnPushPattern(PlayerColor player, squareInd pos) {
    GeneratePawnPrecomputePatterns();
    return kPawnPushesPattern[player][pos];
}

bitboard PrecomputedPiecePatterns::GetPawnAttackPattern(PlayerColor player, squareInd pos) {
    GeneratePawnPrecomputePatterns();
    return kPawnAttacksPattern[player][pos];
}
