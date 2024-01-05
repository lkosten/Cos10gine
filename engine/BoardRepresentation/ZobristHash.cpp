//
// Created by Kostya on 1/5/2024.
//

#include <random>

#include "ZobristHash.h"
#include "BoardRayIterator.h"

ZobristHash::ZobristHash() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> distribution;

    for (auto &i: pieces_hash) {
        for (auto &j: i) {
            j = distribution(gen);
        }
    }

    black_to_move_hash = distribution(gen);

    for (auto &i: castling_rights_hash) {
        i = distribution(gen);
    }

    for (auto &i: valid_en_passant_file_hash) {
        i = distribution(gen);
    }
}


uint64_t ZobristHash::GetBoardHash(const BitBoard &board) {
    uint64_t hash = 0;

    // hash each piece in each square
    for (size_t piece = 0; piece < PieceType::PIECE_TYPE_LEN; ++piece) {
        auto piece_bb = board.GetPiecePositions(static_cast<PieceType>(piece));
        auto pieces = BoardRayIterator::BitscanSquareIndices(piece_bb);

        for (const auto &square: pieces) {
            hash ^= pieces_hash[piece][square];
        }
    }

    // hash for black to move
    if (board.GetPlayerToMove() == PlayerColor::Black) {
        hash ^= black_to_move_hash;
    }

    // hash for castling rights
    if (board.IsWhiteShortCastleAllowed()) {
        hash ^= castling_rights_hash[0];
    }
    if (board.IsWhiteLongCastleAllowed()) {
        hash ^= castling_rights_hash[1];
    }
    if (board.IsBlackShortCastleAllowed()) {
        hash ^= castling_rights_hash[2];
    }
    if (board.IsBlackLongCastleAllowed()) {
        hash ^= castling_rights_hash[3];
    }

    // hash for valid en passant file
    if (board.GetEnPassantAttackSquare() != BitBoard::kNoEnPassantFlag) {
        hash ^= valid_en_passant_file_hash[board.GetEnPassantAttackSquare() & 7];
    }

    return hash;
}

uint64_t ZobristHash::GetMoveChangedZobristHash(const BitBoard &board, const Move &move) {
    uint64_t hash = board.GetZobristHash();

    // hash each piece in each square
    hash ^= pieces_hash[move.source_piece][move.source_square];
    hash ^= pieces_hash[move.source_piece][move.target_square];

    switch (move.type) {
        case MoveType::MoveSimple:
            break;

        case MoveType::CaptureSimple:
            hash ^= pieces_hash[move.target_piece][move.target_square];
            break;

        case MoveType::CaptureEnPassant:
            hash ^= pieces_hash[move.target_piece]
            [move.source_piece == WhitePawn ? move.target_square - 8 : move.target_square + 8];
            break;

        case MoveType::CapturePromotion:
            hash ^= pieces_hash[move.target_piece][move.target_square];
            hash ^= pieces_hash[move.source_piece][move.target_square];
            hash ^= pieces_hash[move.promotion_piece][move.target_square];
            break;

        case MoveType::PromotionSimple:
            hash ^= pieces_hash[move.source_piece][move.target_square];
            hash ^= pieces_hash[move.promotion_piece][move.target_square];
            break;

        case MoveType::CastlingLong:
            if (move.target_piece == PieceType::WhiteKing) {
                hash ^= pieces_hash[PieceType::WhiteRook][0];
                hash ^= pieces_hash[PieceType::WhiteRook][3];
            } else {
                hash ^= pieces_hash[PieceType::BlackRook][56];
                hash ^= pieces_hash[PieceType::BlackRook][59];
            }
            break;

        case MoveType::CastlingShort:
            if (move.target_piece == PieceType::WhiteKing) {
                hash ^= pieces_hash[PieceType::WhiteRook][7];
                hash ^= pieces_hash[PieceType::WhiteRook][5];
            } else {
                hash ^= pieces_hash[PieceType::BlackRook][63];
                hash ^= pieces_hash[PieceType::BlackRook][61];
            }
            break;

        case MOVE_TYPE_LEN:
            break;
    }

    // change player to move
    hash ^= black_to_move_hash;

    // hash for castling rights
    bool castling_rights[4] = {true, true, true, true};
    castling_rights[0] = board.IsWhiteShortCastleAllowed();
    castling_rights[1] = board.IsWhiteLongCastleAllowed();
    castling_rights[2] = board.IsBlackShortCastleAllowed();
    castling_rights[3] = board.IsBlackLongCastleAllowed();

    if (castling_rights[0]) {
        hash ^= castling_rights_hash[0];
    }
    if (board.IsWhiteLongCastleAllowed()) {
        hash ^= castling_rights_hash[1];
    }
    if (board.IsBlackShortCastleAllowed()) {
        hash ^= castling_rights_hash[2];
    }
    if (board.IsBlackLongCastleAllowed()) {
        hash ^= castling_rights_hash[3];
    }

    switch (move.source_piece) {
        case PieceType::WhiteKing:
            castling_rights[1] = false;
            castling_rights[0] = false;
            break;

        case PieceType::BlackKing:
            castling_rights[3] = false;
            castling_rights[2] = false;
            break;

        case PieceType::WhiteRook:
            if (move.source_square == 0) {
                castling_rights[1] = false;
            } else if (move.source_square == 7) {
                castling_rights[0] = false;
            }
            break;

        case PieceType::BlackRook:
            if (move.source_square == 56) {
                castling_rights[3] = false;
            } else if (move.source_square == 63) {
                castling_rights[2] = false;
            }
            break;

        default:
            break;
    }

    if (move.target_piece == PieceType::WhiteRook) {
        if (move.target_square == 0) {
            castling_rights[1] = false;
        } else if (move.target_square == 7) {
            castling_rights[0] = false;
        }
    }
    else if (move.target_piece == PieceType::BlackRook) {
        if (move.target_square == 56) {
            castling_rights[3] = false;
        } else if (move.target_square == 63) {
            castling_rights[2] = false;
        }
    }

    if (castling_rights[0]) {
        hash ^= castling_rights_hash[0];
    }
    if (castling_rights[1]) {
        hash ^= castling_rights_hash[1];
    }
    if (castling_rights[2]) {
        hash ^= castling_rights_hash[2];
    }
    if (castling_rights[3]) {
        hash ^= castling_rights_hash[3];
    }

    // hash for valid en passant file
    if (board.GetEnPassantAttackSquare() != BitBoard::kNoEnPassantFlag) {
        hash ^= valid_en_passant_file_hash[board.GetEnPassantAttackSquare() & 7];
    }
    if (move.type == MoveType::MoveSimple
        && (move.source_piece == PieceType::WhitePawn || move.source_piece == PieceType::BlackPawn)
        &&
        (std::max(move.source_square, move.target_square) - std::min(move.source_square, move.target_square)) == 16) {

        hash ^= valid_en_passant_file_hash[(std::min(move.source_square, move.target_square) + 8) & 7];
    }

    return hash;
}
