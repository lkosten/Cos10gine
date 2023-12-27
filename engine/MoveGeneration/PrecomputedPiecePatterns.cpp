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

std::vector<uint64_t> PrecomputedPiecePatterns::kBishopMagicNumbers = { 12563114949869284214ull, 6134415084254368555ull, 11221545974962354322ull, 12976460313832207120ull, 5822923696126005092ull, 17512789825737118989ull, 10489141587920313665ull, 13956686172688241945ull, 9831804287164514789ull, 8227911301059700901ull, 4609413305929641926ull, 7819041898509815099ull, 10862387483277662554ull, 16690678623116356510ull, 9995820027613481573ull, 2306212735289466236ull, 5176823988360005210ull, 67030301252802231ull, 6462671615508332560ull, 5348459565588253375ull, 11909055103416120019ull, 16894858652535130783ull, 8923615203771425359ull, 759452941579754282ull, 3867116046030348541ull, 11478509260392238960ull, 1689137065981291880ull, 9854769770441107649ull, 3634405999236245104ull, 4965172896744187879ull, 18298586620590001979ull, 3147301687870794725ull, 905593788755108236ull, 4467659872869836371ull, 17504304281428291081ull, 16009996795838894126ull, 8174626797029002018ull, 14114808837603421586ull, 6157692385546266248ull, 9317530139992954424ull, 564089292361648046ull, 4930291060364991151ull, 15043686596675216975ull, 7129659666704934903ull, 7975423646471144448ull, 2532974998439355856ull, 1429632442097408416ull, 13236343913696664844ull, 15368390318100665242ull, 3050820162319417814ull, 9238208355644777928ull, 14920242105704889930ull, 142645090351101441ull, 14078061000895031315ull, 3532338688936174523ull, 12024383868840329243ull, 6545254459170170235ull, 11563050230696806579ull, 15239870182014420131ull, 10850760857901470389ull, 16819907715924496879ull, 5623283950062002713ull, 9332525058396427968ull, 8544376462286823569ull, };
std::vector<uint64_t> PrecomputedPiecePatterns::kBishopMagicNumberFreedomDegrees = { 7, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, 8, 8, 8, 6, 6, 6, 6, 8, 10, 10, 8, 6, 6, 6, 6, 8, 10, 10, 8, 6, 6, 6, 6, 8, 8, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 7, };

std::vector<uint64_t> PrecomputedPiecePatterns::kRookMagicNumbers = { 2672667454208235424ull, 13966809622832896099ull, 14843858392886154230ull, 6917811054800096289ull, 5908712755155694334ull, 1152551376539185731ull, 9217752423498383497ull, 9945087642904747164ull, 15886664985399710181ull, 18223330628238811235ull, 2437613035079468164ull, 1633117857803936747ull, 7398445049051909793ull, 16688651391122015824ull, 12827378979656691748ull, 14574207990021579274ull, 16925558984995789737ull, 5572912785340509356ull, 11754932646189068437ull, 1062681871634225271ull, 6882460787137739167ull, 8869185534997013142ull, 2783611392290608068ull, 2227736041323174949ull, 9156159723694981818ull, 6376041542559507002ull, 5741070084755377880ull, 14119166401045344224ull, 5032772593895686349ull, 5808518352316754515ull, 12801411482931370880ull, 15836929266292577014ull, 15528900786258968542ull, 11817075116853394337ull, 3643686965813114509ull, 2285068944671136563ull, 18435247582679577141ull, 15642126555362629728ull, 18339268122395020418ull, 2409108583046305890ull, 1862592421582381063ull, 10486510727129751511ull, 15557226405806073791ull, 4114835651498429774ull, 16262357930923241488ull, 16913996026945358344ull, 7239680256018411511ull, 8122445934991758908ull, 10107362056531471520ull, 1970458118691208704ull, 13254742402547077955ull, 15363080171481430304ull, 8369785203328312420ull, 9947915754191395328ull, 4125859251132803760ull, 1466597405399073020ull, 11307311502763113246ull, 17606369564009120338ull, 9554951093255870994ull, 14965845035564007518ull, 9644748984753725814ull, 15559848459711727950ull, 1731290665797767948ull, 8426337799128398850ull, };
std::vector<uint64_t> PrecomputedPiecePatterns::kRookMagicNumberFreedomDegrees = { 13, 12, 12, 12, 12, 12, 12, 13, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 13, 12, 12, 12, 12, 12, 12, 13, };