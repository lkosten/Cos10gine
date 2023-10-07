//
// Created by Kostya on 10/7/2023.
//

#include "catch/catch_amalgamated.hpp"

#include "BoardRepresentation/BitBoard.h"
#include "ChessGame.h"

struct PerftResults {
    size_t all_moves;
    size_t captures;
    size_t en_passants;
    size_t castles;
    size_t promotions;
    size_t checks;

    bool operator==(const PerftResults& rhs) const {
        return (all_moves == rhs.all_moves && captures == rhs.captures && en_passants == rhs.en_passants
                && castles == rhs.castles && promotions == rhs.promotions && checks == rhs.checks);
    }

    std::ostream& operator<<(std::ostream& out) const {
        out << "All moves: " << all_moves << '\n';
        out << "Captures: " << captures << '\n';
        out << "En passants: " << en_passants << '\n';
        out << "Castles: " << castles << '\n';
        out << "Promotions: " << promotions << '\n';
        out << "Checks: " << checks << '\n';

        return out;
    }
};

/*
 * All perft positions and results were taken from Chess Programming Wiki:
 * https://www.chessprogramming.org/Perft_Results
 * */

void PlayFromPosition(ChessGame *perft_test_game, std::map<size_t, PerftResults> *perft_results, size_t depth,
                   size_t max_depth) {
    if (depth == max_depth + 1) {
        return;
    }

    auto moves = MoveGenerator::GenerateMoves(perft_test_game->GetLastBoard());
    for (const auto &move: moves) {
        if (perft_test_game->TryMakeMove(move)) {
            switch (move.type) {
                case MoveSimple:
                    ++(*perft_results)[depth].all_moves;
                    break;

                case CaptureSimple:
                    ++(*perft_results)[depth].all_moves;
                    ++(*perft_results)[depth].captures;
                    break;

                case CaptureEnPassant:
                    ++(*perft_results)[depth].all_moves;
                    ++(*perft_results)[depth].captures;
                    ++(*perft_results)[depth].en_passants;
                    break;

                case CapturePromotion:
                    ++(*perft_results)[depth].all_moves;
                    ++(*perft_results)[depth].captures;
                    ++(*perft_results)[depth].promotions;
                    break;

                case PromotionSimple:
                    ++(*perft_results)[depth].all_moves;
                    ++(*perft_results)[depth].promotions;
                    break;

                case CastlingLong:
                case CastlingShort:
                    ++(*perft_results)[depth].all_moves;
                    ++(*perft_results)[depth].castles;
                    break;

                case MOVE_TYPE_LEN:
                default:
                    throw std::runtime_error("Wrong move type\n");
                    break;
            }

            if (MoveGenerator::IsKingInCheck(perft_test_game->GetLastBoard(),
                                             perft_test_game->GetLastBoard().GetPlayerToMove())) {
                ++(*perft_results)[depth].checks;
            }

            PlayFromPosition(perft_test_game, perft_results, depth + 1, max_depth);
            perft_test_game->UnMakeMove();
        }
    }
}

TEST_CASE("Start position perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {20,        0,       0,    0, 0, 0}},
            {2, {400,       0,       0,    0, 0, 0}},
            {3, {8902,      34,      0,    0, 0, 12}},
            {4, {197281,    1576,    0,    0, 0, 469}},
            {5, {4865609,   82719,   258,  0, 0, 27351}},
            {6, {119060324, 2812008, 5248, 0, 0, 809099}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game;
    PlayFromPosition(&game, &calc_perft, 1, 4);

    REQUIRE(calc_perft[1] == right_perft[1]);
    REQUIRE(calc_perft[2] == right_perft[2]);
    REQUIRE(calc_perft[3] == right_perft[3]);
    REQUIRE(calc_perft[4] == right_perft[4]);
}

TEST_CASE("Position 2 perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {48,         8,          0,       2,         0,        0}},
            {2, {2039,       351,        1,       91,        0,        3}},
            {3, {97862,      17102,      45,      3162,      0,        993}},
            {4, {4085603,    757163,     1929,    128013,    15172,    25523}},
            {5, {193690690,  35043416,   73365,   4993637,   8392,     3309887}},
            {6, {8031647685, 1558445089, 3577504, 184513607, 56627920, 92238050}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    PlayFromPosition(&game, &calc_perft, 1, 3);

    REQUIRE(calc_perft[1] == right_perft[1]);
    REQUIRE(calc_perft[2] == right_perft[2]);
    REQUIRE(calc_perft[3] == right_perft[3]);
}

TEST_CASE("Position 3 perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {14,       1,      0,     0, 0,    2}},
            {2, {191,      14,     0,     0, 0,    10}},
            {3, {2812,     209,    2,     0, 0,    267}},
            {4, {43238,    3348,   123,   0, 0,    1680}},
            {5, {674624,   52051,  1165,  0, 0,    52950}},
            {6, {11030083, 940350, 33325, 0, 7552, 452473}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    PlayFromPosition(&game, &calc_perft, 1, 4);

    REQUIRE(calc_perft[1] == right_perft[1]);
    REQUIRE(calc_perft[2] == right_perft[2]);
    REQUIRE(calc_perft[3] == right_perft[3]);
    REQUIRE(calc_perft[4] == right_perft[4]);
}

TEST_CASE("Position 4 perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {6,         0,         0,    0,        0,        0}},
            {2, {264,       87,        0,    6,        48,       10}},
            {3, {9467,      1021,      4,    0,        120,      38}},
            {4, {422333,    131393,    0,    7795,     60032,    15492}},
            {5, {15833292,  2046173,   6512, 0,        329464,   200568}},
            {6, {706045033, 210369132, 212,  10882006, 81102984, 26973664}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    PlayFromPosition(&game, &calc_perft, 1, 4);

    REQUIRE(calc_perft[1] == right_perft[1]);
    REQUIRE(calc_perft[2] == right_perft[2]);
    REQUIRE(calc_perft[3] == right_perft[3]);
    REQUIRE(calc_perft[4] == right_perft[4]);
}

TEST_CASE("Position 5 perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {44,       0, 0, 0, 0, 0}},
            {2, {1486,     0, 0, 0, 0, 0}},
            {3, {62379,    0, 0, 0, 0, 0}},
            {4, {2103487,  0, 0, 0, 0, 0}},
            {5, {89941194, 0, 0, 0, 0, 0}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    PlayFromPosition(&game, &calc_perft, 1, 3);

    REQUIRE(calc_perft[1].all_moves == right_perft[1].all_moves);
    REQUIRE(calc_perft[2].all_moves == right_perft[2].all_moves);
    REQUIRE(calc_perft[3].all_moves == right_perft[3].all_moves);
}

TEST_CASE("Position 6 perft", "[perft]") {
    std::map<size_t, PerftResults> right_perft = {
            {1, {46,         0, 0, 0, 0, 0}},
            {2, {2079,       0, 0, 0, 0, 0}},
            {3, {89890,      0, 0, 0, 0, 0}},
            {4, {3894594,    0, 0, 0, 0, 0}},
            {5, {164075551,  0, 0, 0, 0, 0}},
            {6, {6923051137, 0, 0, 0, 0, 0}},
    };

    std::map<size_t, PerftResults> calc_perft;
    ChessGame game("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    PlayFromPosition(&game, &calc_perft, 1, 3);

    REQUIRE(calc_perft[1].all_moves == right_perft[1].all_moves);
    REQUIRE(calc_perft[2].all_moves == right_perft[2].all_moves);
    REQUIRE(calc_perft[3].all_moves == right_perft[3].all_moves);
}
