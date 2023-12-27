//
// Created by Kostya on 12/27/2023.
//

#include "MagicBitboards.h"
#include "BoardRayIterator.h"

std::vector<std::vector<bitboard>> MagicBitboards::GenerateMagicBitboards(PieceType piece) {
    const auto &magic_number_freedom_degrees = (piece == PieceType::WhiteBishop ? kBishopMagicNumberFreedomDegrees : kRookMagicNumberFreedomDegrees);
    const auto &blocker_bitboards = (piece == PieceType::WhiteBishop ? kBishopBlockerBitboards : kRookBlockerBitboards);
    const auto &magic_numbers = (piece == PieceType::WhiteBishop ? kBishopMagicNumbers : kRookMagicNumbers);

    std::vector<std::vector<bitboard>> magic_bitboards(64);

    for (squareInd square = 0; square < 64; ++square) {
        magic_bitboards[square].assign((1 << magic_number_freedom_degrees[square]), 0);

        size_t value_bits_count = magic_number_freedom_degrees[square] - kFreedomDegreeSupplement;

        for (size_t blocker_variant = 0; blocker_variant < (1 << value_bits_count); ++blocker_variant) {
            size_t current_blocker_variant = blocker_variant;
            std::vector<bitboard> single_bb_blockers = BoardRayIterator::Bitscan(blocker_bitboards[square]);

            // Calculate current blockers bitboard
            bitboard current_blocker_bb = 0;
            for (const auto &blocker_bb : single_bb_blockers) {
                if (current_blocker_variant % 2 == 1) {
                    current_blocker_bb |= blocker_bb;
                }
                current_blocker_variant /= 2;
            }

            // Calculate possible moves with blockers bitboard
            bitboard precalc_possible_moves = 0;
            int dir = (piece == PieceType::WhiteBishop ? RayDirection::NE : RayDirection::E);
            while(dir < RayDirection::NUMBER_OF_DIRECTIONS) {
                BoardRayIterator it(square, static_cast<RayDirection>(dir));

                while(bitboard next_attack = it.GetNextRaySquareBitboard()) {
                    if ((next_attack & current_blocker_bb) != 0) {
                        precalc_possible_moves |= next_attack;
                        break;
                    }

                    precalc_possible_moves |= next_attack;
                }

                dir += 2;
            }

            size_t index = (current_blocker_bb * magic_numbers[square]) >> (64 - magic_number_freedom_degrees[square]);
            magic_bitboards[square][index] = precalc_possible_moves;
        }
    }

    return magic_bitboards;
}

std::vector<bitboard> MagicBitboards::GenerateBlockerBitboards(PieceType piece) {
    std::vector<bitboard> blocker_bitboards(64, 0);

    for (squareInd square = 0; square < 64; ++square) {
        int dir = (piece == PieceType::WhiteBishop ? RayDirection::NE : RayDirection::E);
        while (dir < RayDirection::NUMBER_OF_DIRECTIONS) {
            BoardRayIterator it(square, static_cast<RayDirection>(dir));

            bitboard prev_square_bb = 0;
            while (bitboard next_attack = it.GetNextRaySquareBitboard()) {
                blocker_bitboards[square] |= prev_square_bb;
                prev_square_bb = next_attack;
            }

            dir += 2;
        }
    }

    return blocker_bitboards;
}

const size_t MagicBitboards::kFreedomDegreeSupplement = 1;

bitboard MagicBitboards::GetPieceAttackPattern(PieceType piece, squareInd square, bitboard blockers) {
    bitboard attack_pattern_bb = 0;
    size_t index;

    switch (piece) {
        case WhiteBishop:
        case BlackBishop:
            index = ((blockers & kBishopBlockerBitboards[square]) * kBishopMagicNumbers[square]) >> (64 - kBishopMagicNumberFreedomDegrees[square]);
            attack_pattern_bb |= kBishopMagicBitboards[square][index];
            break;

        case WhiteRook:
        case BlackRook:
            index = ((blockers & kRookBlockerBitboards[square]) * kRookMagicNumbers[square]) >> (64 - kRookMagicNumberFreedomDegrees[square]);
            attack_pattern_bb |= kRookMagicBitboards[square][index];
            break;

        case WhiteQueen:
        case BlackQueen:
            index = ((blockers & kBishopBlockerBitboards[square]) * kBishopMagicNumbers[square]) >> (64 - kBishopMagicNumberFreedomDegrees[square]);
            attack_pattern_bb |= kBishopMagicBitboards[square][index];
            index = ((blockers & kRookBlockerBitboards[square]) * kRookMagicNumbers[square]) >> (64 - kRookMagicNumberFreedomDegrees[square]);
            attack_pattern_bb |= kRookMagicBitboards[square][index];
            break;

        default:
            throw std::runtime_error("Wrong piece passed to magic bitboards");
            break;
    }

    return attack_pattern_bb;
}

std::vector<uint64_t> MagicBitboards::kBishopMagicNumbers = { 12563114949869284214ull, 6134415084254368555ull, 11221545974962354322ull, 12976460313832207120ull, 5822923696126005092ull, 17512789825737118989ull, 10489141587920313665ull, 13956686172688241945ull, 9831804287164514789ull, 8227911301059700901ull, 4609413305929641926ull, 7819041898509815099ull, 10862387483277662554ull, 16690678623116356510ull, 9995820027613481573ull, 2306212735289466236ull, 5176823988360005210ull, 67030301252802231ull, 6462671615508332560ull, 5348459565588253375ull, 11909055103416120019ull, 16894858652535130783ull, 8923615203771425359ull, 759452941579754282ull, 3867116046030348541ull, 11478509260392238960ull, 1689137065981291880ull, 9854769770441107649ull, 3634405999236245104ull, 4965172896744187879ull, 18298586620590001979ull, 3147301687870794725ull, 905593788755108236ull, 4467659872869836371ull, 17504304281428291081ull, 16009996795838894126ull, 8174626797029002018ull, 14114808837603421586ull, 6157692385546266248ull, 9317530139992954424ull, 564089292361648046ull, 4930291060364991151ull, 15043686596675216975ull, 7129659666704934903ull, 7975423646471144448ull, 2532974998439355856ull, 1429632442097408416ull, 13236343913696664844ull, 15368390318100665242ull, 3050820162319417814ull, 9238208355644777928ull, 14920242105704889930ull, 142645090351101441ull, 14078061000895031315ull, 3532338688936174523ull, 12024383868840329243ull, 6545254459170170235ull, 11563050230696806579ull, 15239870182014420131ull, 10850760857901470389ull, 16819907715924496879ull, 5623283950062002713ull, 9332525058396427968ull, 8544376462286823569ull, };
std::vector<uint64_t> MagicBitboards::kBishopMagicNumberFreedomDegrees = { 7, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, 8, 8, 8, 6, 6, 6, 6, 8, 10, 10, 8, 6, 6, 6, 6, 8, 10, 10, 8, 6, 6, 6, 6, 8, 8, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 7, };

std::vector<uint64_t> MagicBitboards::kRookMagicNumbers = { 2672667454208235424ull, 13966809622832896099ull, 14843858392886154230ull, 6917811054800096289ull, 5908712755155694334ull, 1152551376539185731ull, 9217752423498383497ull, 9945087642904747164ull, 15886664985399710181ull, 18223330628238811235ull, 2437613035079468164ull, 1633117857803936747ull, 7398445049051909793ull, 16688651391122015824ull, 12827378979656691748ull, 14574207990021579274ull, 16925558984995789737ull, 5572912785340509356ull, 11754932646189068437ull, 1062681871634225271ull, 6882460787137739167ull, 8869185534997013142ull, 2783611392290608068ull, 2227736041323174949ull, 9156159723694981818ull, 6376041542559507002ull, 5741070084755377880ull, 14119166401045344224ull, 5032772593895686349ull, 5808518352316754515ull, 12801411482931370880ull, 15836929266292577014ull, 15528900786258968542ull, 11817075116853394337ull, 3643686965813114509ull, 2285068944671136563ull, 18435247582679577141ull, 15642126555362629728ull, 18339268122395020418ull, 2409108583046305890ull, 1862592421582381063ull, 10486510727129751511ull, 15557226405806073791ull, 4114835651498429774ull, 16262357930923241488ull, 16913996026945358344ull, 7239680256018411511ull, 8122445934991758908ull, 10107362056531471520ull, 1970458118691208704ull, 13254742402547077955ull, 15363080171481430304ull, 8369785203328312420ull, 9947915754191395328ull, 4125859251132803760ull, 1466597405399073020ull, 11307311502763113246ull, 17606369564009120338ull, 9554951093255870994ull, 14965845035564007518ull, 9644748984753725814ull, 15559848459711727950ull, 1731290665797767948ull, 8426337799128398850ull, };
std::vector<uint64_t> MagicBitboards::kRookMagicNumberFreedomDegrees = { 13, 12, 12, 12, 12, 12, 12, 13, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 11, 12, 13, 12, 12, 12, 12, 12, 12, 13, };

std::vector<bitboard> MagicBitboards::kBishopBlockerBitboards = GenerateBlockerBitboards(PieceType::WhiteBishop);
std::vector<bitboard> MagicBitboards::kRookBlockerBitboards = GenerateBlockerBitboards(PieceType::WhiteRook);

std::vector<std::vector<bitboard>> MagicBitboards::kBishopMagicBitboards = GenerateMagicBitboards(PieceType::WhiteBishop);
std::vector<std::vector<bitboard>> MagicBitboards::kRookMagicBitboards = GenerateMagicBitboards(PieceType::WhiteRook);
