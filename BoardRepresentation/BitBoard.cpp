//
// Created by Kostya on 8/29/2023.
//

#include <vector>
#include "BitBoard.h"

BitBoard BitBoard::GetStartBoard() {
    BitBoard start_board{};
    start_board.f_board[PieceType::whitePawn] = StartPieceBitboard::whitePawnBitboard;
    start_board.f_board[PieceType::whiteBishop] = StartPieceBitboard::whiteBishopBitboard;
    start_board.f_board[PieceType::whiteKnight] = StartPieceBitboard::whiteKnightBitboard;
    start_board.f_board[PieceType::whiteRook] = StartPieceBitboard::whiteRookBitboard;
    start_board.f_board[PieceType::whiteQueen] = StartPieceBitboard::whiteQueenBitboard;
    start_board.f_board[PieceType::whiteKing] = StartPieceBitboard::whiteKingBitboard;

    start_board.f_board[PieceType::blackPawn] = StartPieceBitboard::blackPawnBitboard;
    start_board.f_board[PieceType::blackBishop] = StartPieceBitboard::blackBishopBitboard;
    start_board.f_board[PieceType::blackKnight] = StartPieceBitboard::blackKnightBitboard;
    start_board.f_board[PieceType::blackRook] = StartPieceBitboard::blackRookBitboard;
    start_board.f_board[PieceType::blackQueen] = StartPieceBitboard::blackQueenBitboard;
    start_board.f_board[PieceType::blackKing] = StartPieceBitboard::blackKingBitboard;

    start_board.f_next_turn_player = PlayerColor::white;
    return start_board;
}

bitboard BitBoard::GetPiecePositions(PieceType piece) const {
    return f_board[piece];
}

void BitBoard::DebugDraw(std::ostream &out) {
    std::vector<std::vector<int>> board(8, std::vector<int>(8, -1));

    for (size_t piece = 0; piece < PieceType::PIECE_TYPE_LEN; ++piece) {
        for (size_t square = 0; square < 64; ++square) {
            if ((f_board[piece] >> square) & 1) {
                board[square >> 3][square & 7] = static_cast<int>(piece);
            }
        }
    }

    static std::string chess_symb[PieceType::PIECE_TYPE_LEN] = {"♙", "♘", "♗", "♖", "♕", "♔",
                                                                "♟︎", "♞", "♝", "♜", "♛", "♚"};
    static std::string vertical_delim = "│";
    static std::string empty_square[2] = { "⛊", "⛉" };

    for (size_t rank = 0; rank < 8; ++rank) {
        out << 8 - rank << ' ' << vertical_delim;
        for (size_t file = 0; file < 8; ++file) {
            if (board[rank][file] == -1) {
                out << empty_square[(rank + file) & 1] << vertical_delim;
            }
            else {
                out << chess_symb[board[rank][file]] << vertical_delim;
            }
        }
        out << '\n';
    }
}
