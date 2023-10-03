#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>

#include "BoardRepresentation/BitBoard.h"
#include "BoardRayIterator.h"
#include "ChessGame.h"

#pragma execution_character_set( "utf-8" )

bitboard PrintBoard(const std::vector<std::vector<bool>> &board) {
    bitboard bb = 0;
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 7; file >= 0; --file) {
            int ind_x = rank;
            int ind_y = file;

            bb *= 2;
            std::cout << (board[ind_x][ind_y] ? "1 " : "0 ");
            bb += (board[ind_x][ind_y] ? 1 : 0);
        }
        std::cout << '\n';
    }
    std::cout << bb << '\n';

    return bb;
}

std::vector<bitboard> sliding;
std::vector<bitboard> straight;
std::vector<bitboard> rays;

void GenerateStraightSlidingPattern() {
    int move_x[] = { 1, -1, 0, 0 };
    int move_y[] = { 0, 0, 1, -1 };

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            std::vector<std::vector<bool>> board(8, std::vector<bool>(8, false));

            for (int i = 0; i < 4; ++i) {
               int next_x;
               int next_y;
               next_x = x + move_x[i];
               next_y = y + move_y[i];

               while(next_x >= 0 && next_x < 8 && next_y >= 0 && next_y < 8) {
                   board[next_x][next_y] = true;

                   next_x += move_x[i];
                   next_y += move_y[i];
               }
           }

            std::cout << x + 1 << " " << y + 1 << "\n";
            straight.push_back(PrintBoard(board));
            std::cout << "\n\n";
        }
    }
}

void GenerateDiagonalSlidingPattern() {
    int move_x[] = { 1, 1, -1, -1 };
    int move_y[] = { 1, -1, -1, 1 };

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            std::vector<std::vector<bool>> board(8, std::vector<bool>(8, false));

            for (int i = 0; i < 4; ++i) {
                int next_x;
                int next_y;
                next_x = x + move_x[i];
                next_y = y + move_y[i];

                while(next_x >= 0 && next_x < 8 && next_y >= 0 && next_y < 8) {
                    board[next_x][next_y] = true;

                    next_x += move_x[i];
                    next_y += move_y[i];
                }
            }

            std::cout << x + 1 << " " << y + 1 << "\n";
            sliding.push_back(PrintBoard(board));
            std::cout << "\n\n";
        }
    }
}

void GenerateRays() {
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
                std::cout << x + 1 << " " << y + 1 << " " << i + 1 << "\n";
                rays.push_back(PrintBoard(board));
                std::cout << "\n\n";
            }
        }
    }
}

size_t cnt = 0;
size_t cnt_capt = 0;
size_t cnt_check = 0;
size_t cnt_mate = 0;
size_t cnt_en_p = 0;
size_t cnt_castle = 0;

void MakeFullMoves(ChessGame *perft_test, size_t depth) {
    if (depth == 0) {
        ++cnt;
        return;
    }

    auto moves = MoveGenerator::GenerateMoves(perft_test->GetLastBoard());

    bool mate = true;
    for (const auto &move : moves) {
        if (perft_test->TryMakeMove(move)) {
            if (move.type == MoveType::CaptureSimple && depth == 1) {
                ++cnt_capt;
                if (move.target_piece == WhiteKing || move.target_piece == BlackKing) {
                    std::cout << "\n\n\nSUCK\n\n\n\n\n";
                }
            }
            if (move.type == MoveType::CaptureEnPassant) {
                //perft_test->GetLastBoard().DebugDraw(std::cout);
                //std::cout.flush();
                ++cnt_en_p;
            }
            if (MoveGenerator::IsKingInCheck(perft_test->GetLastBoard(),
                    perft_test->GetLastBoard().GetPlayerToMove())) {
                if (depth == 1) ++cnt_check;
            }
            if (move.type == CastlingLong || move.type == CastlingShort) {
                ++cnt_castle;
            }
            mate = false;

            MakeFullMoves(perft_test, depth - 1);
            perft_test->UnMakeMove();
        }
        else {
        }
    }

    if (mate) {
        ++cnt_mate;
    }

}


int main() {
    SetConsoleOutputCP( 65001 );


    ChessGame perft_test;

    size_t depth;
    std::cin >> depth;
    const auto start{std::chrono::steady_clock::now()};
    MakeFullMoves(&perft_test, depth);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    std::cout << elapsed_seconds.count() << '\n'; // C++20's chrono::duration operator<<
    std::cout << cnt << " moves\n";
    std::cout << cnt_capt << " capture moves\n";
    std::cout << cnt_check << " check moves\n";
    std::cout << cnt_mate << " mate moves\n";
    std::cout << cnt_en_p << " en passant moves\n";
    std::cout << cnt_castle << " castle moves\n";

    return 0;
}
