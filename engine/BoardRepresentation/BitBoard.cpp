//
// Created by Kostya on 8/29/2023.
//

#include <vector>
#include "BitBoard.h"

BitBoard BitBoard::GetStartBoard() {
    BitBoard start_board{};
    start_board.f_board[PieceType::WhitePawn] = StartPieceBitboard::whitePawnBitboard;
    start_board.f_board[PieceType::WhiteBishop] = StartPieceBitboard::whiteBishopBitboard;
    start_board.f_board[PieceType::WhiteKnight] = StartPieceBitboard::whiteKnightBitboard;
    start_board.f_board[PieceType::WhiteRook] = StartPieceBitboard::whiteRookBitboard;
    start_board.f_board[PieceType::WhiteQueen] = StartPieceBitboard::whiteQueenBitboard;
    start_board.f_board[PieceType::WhiteKing] = StartPieceBitboard::whiteKingBitboard;

    start_board.f_board[PieceType::BlackPawn] = StartPieceBitboard::blackPawnBitboard;
    start_board.f_board[PieceType::BlackBishop] = StartPieceBitboard::blackBishopBitboard;
    start_board.f_board[PieceType::BlackKnight] = StartPieceBitboard::blackKnightBitboard;
    start_board.f_board[PieceType::BlackRook] = StartPieceBitboard::blackRookBitboard;
    start_board.f_board[PieceType::BlackQueen] = StartPieceBitboard::blackQueenBitboard;
    start_board.f_board[PieceType::BlackKing] = StartPieceBitboard::blackKingBitboard;

    start_board.f_next_turn_player = PlayerColor::White;
    start_board.f_castling_rights = { true, true, true, true };
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

    static std::string chess_symb[PieceType::PIECE_TYPE_LEN] = { "♟︎", "♞", "♝", "♜", "♛", "♚",
                                                                 "♙", "♘", "♗", "♖", "♕", "♔"};
    static std::string vertical_delim = "│";
    static std::string empty_square[2] = { "⛊", "⛉" };

    for (size_t rank = 0; rank < 8; ++rank) {
        out << 8 - rank << ' ' << vertical_delim;
        for (size_t file = 0; file < 8; ++file) {
            if (board[7 - rank][file] == -1) {
                out << empty_square[(7 - rank + file) & 1] << vertical_delim;
            }
            else {
                out << chess_symb[board[7 - rank][file]] << vertical_delim;
            }
        }
        out << '\n';
    }
    out << '\n';
}

bool BitBoard::MakeMove(const Move &move) {
    bitboard source_piece_board = (1ull << move.source_square);
    bitboard target_piece_board = (1ull << move.target_square);

    f_board[move.source_piece] ^= source_piece_board;
    f_board[move.source_piece] ^= target_piece_board;

    switch (move.type) {
        case MoveType::MoveSimple:
            break;

        case MoveType::CaptureSimple:
            f_board[move.target_piece] ^= target_piece_board;
            break;

        case MoveType::CaptureEnPassant:
            f_board[move.target_piece] ^=
                    (f_next_turn_player == PlayerColor::White ?
                    (target_piece_board >> 8) :
                    (target_piece_board << 8));
            break;

        case MoveType::CapturePromotion:
            f_board[move.target_piece] ^= target_piece_board;
            f_board[move.source_piece] ^= target_piece_board;
            f_board[move.promotion_piece] ^= target_piece_board;
            break;

        case MoveType::PromotionSimple:
            f_board[move.source_piece] ^= target_piece_board;
            f_board[move.promotion_piece] ^= target_piece_board;
            break;

        case MoveType::CastlingLong:
            if (f_next_turn_player == PlayerColor::White) {
                f_board[PieceType::WhiteRook] ^= 1;
                f_board[PieceType::WhiteRook] ^= 8;
            }
            else {
                f_board[PieceType::BlackRook] ^= 72057594037927936;
                f_board[PieceType::BlackRook] ^= 576460752303423488;
            }
            break;

        case MoveType::CastlingShort:
            if (f_next_turn_player == PlayerColor::White) {
                f_board[PieceType::WhiteRook] ^= 128;
                f_board[PieceType::WhiteRook] ^= 32;
            }
            else {
                f_board[PieceType::BlackRook] ^= 9223372036854775808ull;
                f_board[PieceType::BlackRook] ^= 2305843009213693952ull;
            }
            break;

        case MOVE_TYPE_LEN:
            break;
    }

    f_next_turn_player = static_cast<PlayerColor>(!f_next_turn_player);

    SetEnPassantRule(move);
    SetCastlingRights(move);
    return !MoveGenerator::IsKingInCheck(*this, static_cast<PlayerColor>(!f_next_turn_player));
}

void BitBoard::UnMakeMove(const Move &move) {
    bitboard source_piece_board = (1ull << move.source_square);
    bitboard target_piece_board = (1ull << move.target_square);

    f_board[move.source_piece] ^= source_piece_board;
    f_board[move.source_piece] ^= target_piece_board;

    switch (move.type) {
        case MoveType::MoveSimple:
            break;

        case MoveType::CaptureSimple:
            f_board[move.target_piece] ^= target_piece_board;
            break;

        case MoveType::CaptureEnPassant:
            f_board[move.target_piece] ^=
                    (f_next_turn_player == PlayerColor::White ?
                     (target_piece_board >> 8) :
                     (target_piece_board << 8));
            break;

        case MoveType::CapturePromotion:
            f_board[move.target_piece] ^= target_piece_board;
            f_board[move.source_piece] ^= target_piece_board;
            f_board[move.promotion_piece] ^= target_piece_board;
            break;

        case MoveType::PromotionSimple:
            f_board[move.source_piece] ^= target_piece_board;
            f_board[move.promotion_piece] ^= target_piece_board;
            break;

        case MoveType::CastlingLong:
            if (f_next_turn_player == PlayerColor::White) {
                f_board[PieceType::WhiteRook] ^= 1;
                f_board[PieceType::WhiteRook] ^= 8;
            } else {
                f_board[PieceType::BlackRook] ^= 72057594037927936;
                f_board[PieceType::BlackRook] ^= 576460752303423488;
            }
            break;

        case MoveType::CastlingShort:
            if (f_next_turn_player == PlayerColor::White) {
                f_board[PieceType::WhiteRook] ^= 128;
                f_board[PieceType::WhiteRook] ^= 32;
            } else {
                f_board[PieceType::BlackRook] ^= 9223372036854775808ull;
                f_board[PieceType::BlackRook] ^= 2305843009213693952ull;
            }
            break;

        case MOVE_TYPE_LEN:
            break;
    }
    f_next_turn_player = static_cast<PlayerColor>(!f_next_turn_player);
}

PieceType BitBoard::GetPieceTypeBySquare(std::uint64_t square_bb) const {
    for (size_t type = PieceType::WhitePawn; type < PieceType::PIECE_TYPE_LEN; ++type) {
        if ((f_board[type] & square_bb) != 0) {
            return static_cast<PieceType>(type);
        }
    }

    return PieceType::PIECE_TYPE_LEN;
}

PieceType BitBoard::GetPieceTypeBySquare(squareInd square) const {
    return GetPieceTypeBySquare(1ull << square);
}

void BitBoard::SetEnPassantRule(const Move &move) {
    if (move.type == MoveType::MoveSimple
        && (move.source_piece == PieceType::WhitePawn || move.source_piece == PieceType::BlackPawn)
        && (std::max(move.source_square, move.target_square) - std::min(move.source_square, move.target_square)) == 16) {

        f_en_passant_square = std::min(move.source_square, move.target_square) + 8;
    }
    else {
        f_en_passant_square = kNoEnPassantFlag;
    }
}

void BitBoard::SetCastlingRights(const Move &move) {
    switch (move.source_piece) {
        case PieceType::WhiteKing:
            f_castling_rights.white_long_catle = false;
            f_castling_rights.white_short_catle = false;
            break;

        case PieceType::BlackKing:
            f_castling_rights.black_long_catle = false;
            f_castling_rights.black_short_catle = false;
            break;

        case PieceType::WhiteRook:
            if (move.source_square == 0) {
                f_castling_rights.white_long_catle = false;
            }
            else if (move.source_square == 7) {
                f_castling_rights.white_short_catle = false;
            }
            break;

        case PieceType::BlackRook:
            if (move.source_square == 56) {
                f_castling_rights.black_long_catle = false;
            }
            else if (move.source_square == 63) {
                f_castling_rights.black_short_catle = false;
            }
            break;

        default:
            break;
    }

    if ((f_board[WhiteRook] & 1) == 0) {
        f_castling_rights.white_long_catle = false;
    }
    if ((f_board[WhiteRook] & (1ull << 7)) == 0) {
        f_castling_rights.white_short_catle = false;
    }
    if ((f_board[BlackRook] & (1ull << 56)) == 0) {
        f_castling_rights.black_long_catle = false;
    }
    if ((f_board[BlackRook] & (1ull << 63)) == 0) {
        f_castling_rights.black_short_catle = false;
    }
}

bool BitBoard::IsWhiteLongCastleAllowed() const {
    return f_castling_rights.white_long_catle;
}

bool BitBoard::IsWhiteShortCastleAllowed() const {
    return f_castling_rights.white_short_catle;
}

bool BitBoard::IsBlackLongCastleAllowed() const {
    return f_castling_rights.black_long_catle;
}

bool BitBoard::IsBlackShortCastleAllowed() const {
    return f_castling_rights.black_short_catle;
}

squareInd BitBoard::GetEnPassantAttackSquare() const {
    return f_en_passant_square;
}
