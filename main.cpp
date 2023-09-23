#include <iostream>
#include <fstream>
#include <windows.h>

#include "BoardRepresentation/BitBoard.h"

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

int main() {
    SetConsoleOutputCP( 65001 );

    /*BitBoard bb{};
    bb = BitBoard::GetStartBoard();
    bb.DebugDraw(std::cout);
    std::cout << std::endl;
    std::cout << std::endl;

    auto moves = MoveGenerator::GenerateMoves(bb);

    for (const auto &move : moves) {
        bb.MakeMove(move);

        bb.DebugDraw(std::cout);
        std::cout << std::endl;
        bb.UnMakeMove(move);
    }*/

    GenerateStraightSlidingPattern();
    GenerateDiagonalSlidingPattern();

    std::cout << "STRAIGHT";
    std::cout << "{\n";
    for (const auto &i : straight) {
        std::cout << i << "ull,\n";
    }
    std::cout << "}\n\n\n\n";

    std::cout << "SLIDING";
    std::cout << "{\n";
    for (const auto &i : sliding) {
        std::cout << i << "ull,\n";
    }
    std::cout << "}\n";
    return 0;
}
