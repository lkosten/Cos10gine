//
// Created by Kostya on 9/24/2023.
//

#include "BoardRays.h"

std::vector<std::vector<bitboard>> BoardRays::kRayPatterns;

bitboard BoardRays::GetRayDirection(const squareInd index, const RayDirection direction) {
    GeneratrePrecomputeRays();
    return kRayPatterns[index][direction];
}

BoardRays::BoardRays() {
    GeneratrePrecomputeRays();
}

void BoardRays::GeneratrePrecomputeRays() {
    if (!kRayPatterns.empty()) {
        return;
    }
    kRayPatterns.resize(64, std::vector<bitboard>(8, 0));

    int move_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    int move_y[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

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
