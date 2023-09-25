//
// Created by Kostya on 9/24/2023.
//

#include "BoardRayIterator.h"

std::vector<std::vector<bitboard>> BoardRayIterator::kRayPatterns;

BoardRayIterator::BoardRayIterator() : ray_direction(), current_ray() {
    GeneratrePrecomputeRays();
}

BoardRayIterator::BoardRayIterator(squareInd index, RayDirection direction) : ray_direction(), current_ray() {
    GeneratrePrecomputeRays();
    ray_direction = direction;
    current_ray = kRayPatterns[index][direction];
}

bitboard BoardRayIterator::GetRayByDirection(squareInd index, RayDirection direction) {
    GeneratrePrecomputeRays();
    return kRayPatterns[index][direction];
}

void BoardRayIterator::GeneratrePrecomputeRays() {
    if (!kRayPatterns.empty()) {
        return;
    }
    kRayPatterns.resize(64, std::vector<bitboard>(8, 0));

    int move_y[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int move_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            for (int i = 0; i < 8; ++i) {
                std::vector<std::vector<bool>> board(8, std::vector<bool>(8, false));

                int next_x;
                int next_y;
                next_x = x + move_x[i];
                next_y = y + move_y[i];

                while(next_x >= 0 && next_x < 8 && next_y >= 0 && next_y < 8) {
                    board[next_x][next_y] = true;

                    next_x += move_x[i];
                    next_y += move_y[i];
                }

                bitboard bb = 0;
                for (int rank = 7; rank >= 0; --rank) {
                    for (int file = 7; file >= 0; --file) {
                        int ind_x = rank;
                        int ind_y = file;

                        bb *= 2;
                        bb += (board[ind_x][ind_y] ? 1 : 0);
                    }
                }

                kRayPatterns[x * 8 + y][i] = bb;
            }
        }
    }
}

void BoardRayIterator::SetBoardRay(squareInd index, RayDirection direction) {
    ray_direction = direction;
    current_ray = kRayPatterns[index][direction];
}

bitboard BoardRayIterator::GetNextRaySquareBitboard() {
    bitboard next_ray_square_bb = 0;

    // LS1B
    if (ray_direction <= NW) {
        next_ray_square_bb = LS1B(current_ray);
    }
    else { // MS1B
        next_ray_square_bb = MS1B(current_ray);
    }

    current_ray = (current_ray ^ next_ray_square_bb);
    return next_ray_square_bb;
}

bitboard BoardRayIterator::LS1B(bitboard bb) {
    if (bb == 0) {
        return 0;
    }

    return (bb & (~(bb - 1)));
}

bitboard BoardRayIterator::MS1B(bitboard bb) {
    if (bb == 0) {
        return 0;
    }

    static const int index64[64] = {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
    };
    static const bitboard debruijn64 = 0x03f79d71b4cb0a89;

    bb |= bb >> 1;
    bb |= bb >> 2;
    bb |= bb >> 4;
    bb |= bb >> 8;
    bb |= bb >> 16;
    bb |= bb >> 32;
    return (1ull << index64[(bb * debruijn64) >> 58]);
}

