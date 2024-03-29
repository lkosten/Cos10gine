//
// Created by Kostya on 8/29/2023.
//

#ifndef LCOS10GINE_BITBOARD_H
#define LCOS10GINE_BITBOARD_H

#include <cstdint>
#include <fstream>

typedef std::uint64_t bitboard;
typedef std::uint8_t squareInd;

#include "PieceDefines.h"
#include "MoveGeneration/MoveGenerator.h"
#include "Interfaces/FEN.h"
#include "ZobristHash.h"

class Move;

/*
 * LSF mapping (Least Significant File)
 * LSF squareIndex = 8 * rankIndex + fileIndex
 * FileIndex   = squareIndex modulo 8  = squareIndex & 7
 * RankIndex   = squareIndex div    8  = squareIndex >> 3
 *
 * Little-endian mapping (LERF)
 * a < h, 0 < 7
 *
 *   a1, b1, c1, d1, e1, f1, g1, h1,
 *   a2, b2, c2, d2, e2, f2, g2, h2,
 *   a3, b3, c3, d3, e3, f3, g3, h3,
 *   a4, b4, c4, d4, e4, f4, g4, h4,
 *   a5, b5, c5, d5, e5, f5, g5, h5,
 *   a6, b6, c6, d6, e6, f6, g6, h6,
 *   a7, b7, c7, d7, e7, f7, g7, h7,
 *   a8, b8, c8, d8, e8, f8, g8, h8
 *
 * a-file             0x0101010101010101
 * h-file             0x8080808080808080
 * 1st rank           0x00000000000000FF
 * 8th rank           0xFF00000000000000
 * a1-h8 diagonal     0x8040201008040201
 * h1-a8 antidiagonal 0x0102040810204080
 * light squares      0x55AA55AA55AA55AA
 * dark squares       0xAA55AA55AA55AA55
 */

class BitBoard {
public:
    BitBoard() = default;

    static BitBoard GetStartBoard();

    bitboard GetPiecePositions(PieceType piece) const;
    PlayerColor GetPlayerToMove() const { return f_next_turn_player; }

    bool MakeMove(const Move &move);

    void DebugDraw(std::ostream &out);

    PieceType GetPieceTypeBySquare(bitboard square_bb) const;
    PieceType GetPieceTypeBySquare(squareInd square) const;

    squareInd GetEnPassantAttackSquare() const;

    bool IsWhiteLongCastleAllowed() const;
    bool IsWhiteShortCastleAllowed() const;
    bool IsBlackLongCastleAllowed() const;
    bool IsBlackShortCastleAllowed() const;

    uint64_t GetZobristHash() const;
    uint32_t GetNumberOfMoves() const;

    bool IsThreefoldRepetition() const;

    static const bitboard kAFileBitboard = 0x0101010101010101;
    static const bitboard kBFileBitboard = 0x0202020202020202;
    static const bitboard kGFileBitboard = 0x4040404040404040;
    static const bitboard kHFileBitboard = 0x8080808080808080;

    static const bitboard k1RankBitboard = 0x00000000000000FF;
    static const bitboard k2RankBitboard = 0x000000000000FF00;
    static const bitboard k7RankBitboard = 0x00FF000000000000;
    static const bitboard k8RankBitboard = 0xFF00000000000000;

    static const squareInd kNoEnPassantFlag = 200;

private:
    bitboard f_board[PieceType::PIECE_TYPE_LEN];
    PlayerColor f_next_turn_player;
    squareInd f_en_passant_square;

    struct CastlingRights {
        bool white_long_catle;
        bool white_short_catle;

        bool black_long_catle;
        bool black_short_catle;

    } f_castling_rights;

    uint64_t f_zobrist_hash;
    std::vector<uint64_t> f_repetition_table;
    bool f_is_threefold_repetition;

    void SetEnPassantRule(const Move &move);
    void SetCastlingRights(const Move &move);

    friend class FEN;
};


enum StartPieceBitboard : bitboard {
    whitePawnBitboard = 65280,
    whiteKnightBitboard = 66,
    whiteBishopBitboard = 36,
    whiteRookBitboard = 129,
    whiteQueenBitboard = 8,
    whiteKingBitboard = 16,

    blackPawnBitboard = 71776119061217280,
    blackKnightBitboard = 4755801206503243776,
    blackBishopBitboard = 2594073385365405696,
    blackRookBitboard = 9295429630892703744ull,
    blackQueenBitboard = 576460752303423488,
    blackKingBitboard = 1152921504606846976,
};

#endif //LCOS10GINE_BITBOARD_H
