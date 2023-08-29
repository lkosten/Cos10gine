//
// Created by Kostya on 8/29/2023.
//

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

bitboard BitBoard::GetPiecePositions(PieceType piece) {
    return f_board[piece];
}
