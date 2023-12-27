//
// Created by Kostya on 10/9/2023.
//

#include "PrecomputedPiecePatterns.h"

std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::GeneratePawnPushesPrecomputePatterns() {
    std::vector<std::vector<bitboard>> pattern(PLAYER_NUMBER, std::vector<bitboard>(64, 0));

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);
        bitboard pushes_bb = 0;

        if ((square_bb & BitBoard::k8RankBitboard) == 0) {
            pushes_bb |= (square_bb << 8);
        }
        if ((square_bb & BitBoard::k2RankBitboard) != 0) {
            pushes_bb |= (square_bb << 16);
        }

        pattern[PlayerColor::White][square] = pushes_bb;
    }

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);
        bitboard pushes_bb = 0;

        if ((square_bb & BitBoard::k1RankBitboard) == 0) {
            pushes_bb |= (square_bb >> 8);
        }
        if ((square_bb & BitBoard::k7RankBitboard) != 0) {
            pushes_bb |= (square_bb >> 16);
        }

        pattern[PlayerColor::Black][square] = pushes_bb;
    }

    return pattern;
}

std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::GeneratePawnAttacksPrecomputePatterns() {
    std::vector<std::vector<bitboard>> pattern(PLAYER_NUMBER, std::vector<bitboard>(64, 0));

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb << 7);
        }
        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb << 9);
        }

        pattern[PlayerColor::White][square] = attacks_bb;
    }

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 9);
        }
        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 7);
        }

        pattern[PlayerColor::Black][square] = attacks_bb;
    }

    return pattern;
}

std::vector<bitboard> PrecomputedPiecePatterns::GenerateKnightPrecomputePatterns() {
    std::vector<bitboard> pattern(64, 0ull);

    for (squareInd square = 0; square < 64; ++square) {
        bitboard attack_pattern_bb = 0;
        bitboard knight_bb = (1ull << square);

        // ind + 15
        if ((knight_bb & BitBoard::kAFileBitboard) == 0
            && (knight_bb & (BitBoard::k7RankBitboard | BitBoard::k8RankBitboard)) == 0) {

            attack_pattern_bb |= (knight_bb << 15);
        }

        // ind + 17
        if ((knight_bb & BitBoard::kHFileBitboard) == 0
            && (knight_bb & (BitBoard::k7RankBitboard | BitBoard::k8RankBitboard)) == 0) {

            attack_pattern_bb |= (knight_bb << 17);
        }

        // ind + 6
        if ((knight_bb & (BitBoard::kAFileBitboard | BitBoard::kBFileBitboard)) == 0
            && (knight_bb & BitBoard::k8RankBitboard) == 0) {

            attack_pattern_bb |= (knight_bb << 6);
        }

        // ind + 10
        if ((knight_bb & (BitBoard::kGFileBitboard | BitBoard::kHFileBitboard)) == 0
            && (knight_bb & BitBoard::k8RankBitboard) == 0) {

            attack_pattern_bb |= (knight_bb << 10);
        }

        // ind - 10
        if ((knight_bb & (BitBoard::kAFileBitboard | BitBoard::kBFileBitboard)) == 0
            && (knight_bb & BitBoard::k1RankBitboard) == 0) {

            attack_pattern_bb |= (knight_bb >> 10);
        }

        // ind - 6
        if ((knight_bb & (BitBoard::kGFileBitboard | BitBoard::kHFileBitboard)) == 0
            && (knight_bb & BitBoard::k1RankBitboard) == 0) {

            attack_pattern_bb |= (knight_bb >> 6);
        }

        // ind - 17
        if ((knight_bb & BitBoard::kAFileBitboard) == 0
            && (knight_bb & (BitBoard::k2RankBitboard | BitBoard::k1RankBitboard)) == 0) {

            attack_pattern_bb |= (knight_bb >> 17);
        }

        // ind - 15
        if ((knight_bb & BitBoard::kHFileBitboard) == 0
            && (knight_bb & (BitBoard::k2RankBitboard | BitBoard::k1RankBitboard)) == 0) {

            attack_pattern_bb |= (knight_bb >> 15);
        }

        pattern[square] = attack_pattern_bb;
    }

    return pattern;
}

std::vector<bitboard> PrecomputedPiecePatterns::GenerateKingPrecomputePatterns() {
    std::vector<bitboard> pattern(64, 0ull);

    for (squareInd square = 0; square < 64; ++square) {
        bitboard attack_pattern_bb = 0;
        bitboard king_bb = (1ull << square);

        // ind - 1
        if ((king_bb & BitBoard::kAFileBitboard) == 0) {
            attack_pattern_bb |= (king_bb >> 1);
        }

        // ind + 1
        if ((king_bb & BitBoard::kHFileBitboard) == 0) {
            attack_pattern_bb |= (king_bb << 1);
        }

        // ind - 8
        if ((king_bb & BitBoard::k1RankBitboard) == 0) {
            attack_pattern_bb |= (king_bb >> 8);
        }

        // ind + 8
        if ((king_bb & BitBoard::k8RankBitboard) == 0) {
            attack_pattern_bb |= (king_bb << 8);
        }

        // ind + 7
        if ((king_bb & BitBoard::kAFileBitboard) == 0
            && (king_bb & BitBoard::k8RankBitboard) == 0) {

            attack_pattern_bb |= (king_bb << 7);
        }

        // ind + 9
        if ((king_bb & BitBoard::kHFileBitboard) == 0
            && (king_bb & BitBoard::k8RankBitboard) == 0) {

            attack_pattern_bb |= (king_bb << 9);
        }

        // ind - 9
        if ((king_bb & BitBoard::kAFileBitboard) == 0
            && (king_bb & BitBoard::k1RankBitboard) == 0) {

            attack_pattern_bb |= (king_bb >> 9);
        }

        // ind - 7
        if ((king_bb & BitBoard::kHFileBitboard) == 0
            && (king_bb & BitBoard::k1RankBitboard) == 0) {

            attack_pattern_bb |= (king_bb >> 7);
        }

        pattern[square] = attack_pattern_bb;
    }

    return pattern;
}

bitboard PrecomputedPiecePatterns::GetKnightAttackPattern(squareInd pos) {
    return kKnightAttacksPattern[pos];
}

bitboard PrecomputedPiecePatterns::GetKingAttackPattern(squareInd pos) {
    return kKingAttacksPattern[pos];
}

bitboard PrecomputedPiecePatterns::GetPawnPushPattern(PlayerColor player, squareInd pos) {
    return kPawnPushesPattern[player][pos];
}

bitboard PrecomputedPiecePatterns::GetPawnAttackPattern(PlayerColor player, squareInd pos) {
    return kPawnAttacksPattern[player][pos];
}

std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::kPawnPushesPattern = GeneratePawnPushesPrecomputePatterns();
std::vector<std::vector<bitboard>> PrecomputedPiecePatterns::kPawnAttacksPattern = GeneratePawnAttacksPrecomputePatterns();
std::vector<bitboard> PrecomputedPiecePatterns::kKnightAttacksPattern = GenerateKnightPrecomputePatterns();
std::vector<bitboard> PrecomputedPiecePatterns::kKingAttacksPattern = GenerateKingPrecomputePatterns();
