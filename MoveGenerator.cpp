//
// Created by Kostya on 9/2/2023.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::GenerateMoves(const BitBoard& board) {
    std::vector<Move> all_moves;

    GenerateWhitePawnMoves(board, &all_moves);
    GenerateBlackPawnMoves(board, &all_moves);

    GenerateWhiteKnightMoves(board, &all_moves);
    GenerateBlackKnightMoves(board, &all_moves);

    return all_moves;
}

bitboard MoveGenerator::GenerateOccupiedPositions(const BitBoard &board) {
    bitboard occupied = 0;
    for (size_t ind = 0; ind < PieceType::PIECE_TYPE_LEN; ++ind) {
        occupied |= board.GetPiecePositions(static_cast<PieceType>(ind));
    }

    return occupied;
}

bitboard MoveGenerator::GenerateBlackOccupiedPositions(const BitBoard &board) {
    bitboard occupied = 0;
    for (size_t ind = BlackPawn; ind < PieceType::PIECE_TYPE_LEN; ++ind) {
        occupied |= board.GetPiecePositions(static_cast<PieceType>(ind));
    }

    return occupied;
}

bitboard MoveGenerator::GenerateWhiteOccupiedPositions(const BitBoard &board) {
    bitboard occupied = 0;
    for (size_t ind = PieceType::WhitePawn; ind < PieceType::BlackPawn; ++ind) {
        occupied |= board.GetPiecePositions(static_cast<PieceType>(ind));
    }

    return occupied;
}

void MoveGenerator::GenerateWhitePawnMoves(const BitBoard &board, std::vector<Move> *all_moves) {
    bitboard occupied_positions = GenerateOccupiedPositions(board);
    bitboard black_occupied_positions = GenerateBlackOccupiedPositions(board);

    for (std::uint8_t ind = 0; ind < 64; ++ind) {
        if (((board.GetPiecePositions(PieceType::WhitePawn) >> ind) & 1) == 0) {
            continue;
        }

        // one square push
        if ((occupied_positions & (1ull << (ind + 8))) == 0) {
            Move move{};
            move.source_square = ind;
            move.target_square = ind + 8;

            move.type = MoveSimple;

            move.source_piece = WhitePawn;
            move.target_piece = WhitePawn;

            // promotion
            if (((1ull << ind) & StartPieceBitboard::blackPawnBitboard) != 0) {
                move.type = MoveType::PromotionSimple;

                for (size_t piece = 1; piece < BlackPawn; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::MoveSimple;
                move.promotion_piece = WhitePawn;

                all_moves->push_back(move);
            }

        }

        // two squares push
        if ((StartPieceBitboard::whitePawnBitboard & (1ull << ind)) != 0
            && (occupied_positions & (1ull << (ind + 8))) == 0
            && (occupied_positions & (1ull << (ind + 16))) == 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind + 16;

            move.type = MoveSimple;

            move.source_piece = WhitePawn;
            move.target_piece = WhitePawn;

            move.promotion_piece = WhitePawn;

            all_moves->push_back(move);
        }

        // left diagonal capture
        if (((1ull << ind) & BitBoard::kAFileBitboard) == 0
            && ((1ull << (ind + 7)) & black_occupied_positions) != 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind + 7;

            move.type = MoveType::CaptureSimple;

            move.source_piece = WhitePawn;
            move.target_piece = board.GetPieceTypeBySquare(move.target_square);

            // promotion capture
            if (((1ull << ind) & StartPieceBitboard::blackPawnBitboard) != 0) {
                move.type = MoveType::CapturePromotion;

                for (size_t piece = 1; piece < BlackPawn; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::CaptureSimple;
                move.promotion_piece = WhitePawn;

                all_moves->push_back(move);
            }
        }

        // right diagonal capture
        if (((1ull << ind) & BitBoard::kHFileBitboard) == 0
            && ((1ull << (ind + 9)) & black_occupied_positions) != 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind + 9;

            move.type = MoveType::CaptureSimple;

            move.source_piece = WhitePawn;
            move.target_piece = board.GetPieceTypeBySquare(move.target_square);

            // promotion capture
            if (((1ull << ind) & StartPieceBitboard::blackPawnBitboard) != 0) {
                move.type = MoveType::CapturePromotion;

                for (size_t piece = 1; piece < BlackPawn; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::CaptureSimple;
                move.promotion_piece = WhitePawn;

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateBlackPawnMoves(const BitBoard &board, std::vector<Move> *all_moves) {
    bitboard occupied_positions = GenerateOccupiedPositions(board);
    bitboard white_occupied_positions = GenerateWhiteOccupiedPositions(board);

    for (std::uint8_t ind = 0; ind < 64; ++ind) {
        if (((board.GetPiecePositions(PieceType::BlackPawn) >> ind) & 1) == 0) {
            continue;
        }

        // one square push
        if ((occupied_positions & (1ull << (ind - 8))) == 0) {
            Move move{};
            move.source_square = ind;
            move.target_square = ind - 8;

            move.type = MoveSimple;

            move.source_piece = BlackPawn;
            move.target_piece = BlackPawn;

            // promotion
            if (((1ull << ind) & StartPieceBitboard::whitePawnBitboard) != 0) {
                move.type = MoveType::PromotionSimple;

                for (size_t piece = BlackPawn + 1; piece < PIECE_TYPE_LEN; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::MoveSimple;
                move.promotion_piece = BlackPawn;

                all_moves->push_back(move);
            }

        }

        // two squares push
        if ((StartPieceBitboard::blackPawnBitboard & (1ull << ind)) != 0
            && (occupied_positions & (1ull << (ind - 8))) == 0
            && (occupied_positions & (1ull << (ind - 16))) == 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind - 16;

            move.type = MoveSimple;

            move.source_piece = BlackPawn;
            move.target_piece = BlackPawn;

            move.promotion_piece = BlackPawn;

            all_moves->push_back(move);
        }

        // left diagonal capture
        if (((1ull << ind) & BitBoard::kAFileBitboard) == 0
            && ((1ull << (ind - 7)) & white_occupied_positions) != 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind - 7;

            move.type = MoveType::CaptureSimple;

            move.source_piece = BlackPawn;
            move.target_piece = board.GetPieceTypeBySquare(move.target_square);

            // promotion capture
            if (((1ull << ind) & StartPieceBitboard::whitePawnBitboard) != 0) {
                move.type = MoveType::CapturePromotion;

                for (size_t piece = BlackPawn + 1; piece < PIECE_TYPE_LEN; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::CaptureSimple;
                move.promotion_piece = BlackPawn;

                all_moves->push_back(move);
            }
        }

        // right diagonal capture
        if (((1ull << ind) & BitBoard::kHFileBitboard) == 0
            && ((1ull << (ind - 9)) & white_occupied_positions) != 0) {

            Move move{};
            move.source_square = ind;
            move.target_square = ind - 9;

            move.type = MoveType::CaptureSimple;

            move.source_piece = BlackPawn;
            move.target_piece = board.GetPieceTypeBySquare(move.target_square);

            // promotion capture
            if (((1ull << ind) & StartPieceBitboard::whitePawnBitboard) != 0) {
                move.type = MoveType::CapturePromotion;

                for (size_t piece = BlackPawn + 1; piece < PIECE_TYPE_LEN; ++piece) {
                    move.promotion_piece = static_cast<PieceType>(piece);

                    all_moves->push_back(move);
                }
            }
            else { // non promotion
                move.type = MoveType::CaptureSimple;
                move.promotion_piece = BlackPawn;

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateWhiteKnightMoves(const BitBoard &board, std::vector<Move> *all_moves) {
    bitboard white_occupied_positions = GenerateWhiteOccupiedPositions(board);
    bitboard black_occupied_positions = GenerateBlackOccupiedPositions(board);

    for (std::uint8_t ind = 0; ind < 64; ++ind) {
        if (((board.GetPiecePositions(PieceType::WhiteKnight) >> ind) & 1) == 0) {
            continue;
        }

        auto attack_pattern = GenerateKnightAttackPattern(ind);

        for (const auto &attack_square : attack_pattern) {
            if (((1ull << attack_square) & white_occupied_positions) != 0) {
                continue;
            }

            Move move{};
            move.source_square = ind;
            move.target_square = attack_square;

            move.source_piece = WhiteKnight;
            move.promotion_piece = WhiteKnight;

            // capturing
            if (((1ull << attack_square) & black_occupied_positions) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateBlackKnightMoves(const BitBoard &board, std::vector<Move> *all_moves) {
    bitboard white_occupied_positions = GenerateWhiteOccupiedPositions(board);
    bitboard black_occupied_positions = GenerateBlackOccupiedPositions(board);

    for (std::uint8_t ind = 0; ind < 64; ++ind) {
        if (((board.GetPiecePositions(PieceType::BlackKnight) >> ind) & 1) == 0) {
            continue;
        }

        auto attack_pattern = GenerateKnightAttackPattern(ind);

        for (const auto &attack_square : attack_pattern) {
            if (((1ull << attack_square) & black_occupied_positions) != 0) {
                continue;
            }

            Move move{};
            move.source_square = ind;
            move.target_square = attack_square;

            move.source_piece = BlackKnight;
            move.promotion_piece = BlackKnight;

            // capturing
            if (((1ull << attack_square) & white_occupied_positions) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
        }
    }
}


std::vector<std::uint8_t> MoveGenerator::GenerateKnightAttackPattern(std::uint8_t knight_pos) {
    std::vector<std::uint8_t> attack_pattern;
    attack_pattern.reserve(8);

    bitboard knight_bb = (1ull << knight_pos);

    // ind + 15
    if ((knight_bb & BitBoard::kAFileBitboard) == 0
        && (knight_bb & (BitBoard::k7RankBitboard | BitBoard::k8RankBitboard)) == 0) {

        attack_pattern.push_back(knight_pos + 15);
    }

    // ind + 17
    if ((knight_bb & BitBoard::kHFileBitboard) == 0
        && (knight_bb & (BitBoard::k7RankBitboard | BitBoard::k8RankBitboard)) == 0) {

        attack_pattern.push_back(knight_pos + 17);
    }

    // ind + 6
    if ((knight_bb & (BitBoard::kAFileBitboard | BitBoard::kBFileBitboard)) == 0
        && (knight_bb & BitBoard::k8RankBitboard) == 0) {

        attack_pattern.push_back(knight_pos + 6);
    }

    // ind + 10
    if ((knight_bb & (BitBoard::kGFileBitboard | BitBoard::kHFileBitboard)) == 0
        && (knight_bb & BitBoard::k8RankBitboard) == 0) {

        attack_pattern.push_back(knight_pos + 10);
    }

    // ind - 10
    if ((knight_bb & (BitBoard::kAFileBitboard | BitBoard::kBFileBitboard)) == 0
        && (knight_bb & BitBoard::k1RankBitboard) == 0) {

        attack_pattern.push_back(knight_pos - 10);
    }

    // ind - 6
    if ((knight_bb & (BitBoard::kGFileBitboard | BitBoard::kHFileBitboard)) == 0
        && (knight_bb & BitBoard::k1RankBitboard) == 0) {

        attack_pattern.push_back(knight_pos - 6);
    }

    // ind - 17
    if ((knight_bb & BitBoard::kAFileBitboard) == 0
        && (knight_bb & (BitBoard::k2RankBitboard | BitBoard::k1RankBitboard)) == 0) {

        attack_pattern.push_back(knight_pos - 17);
    }

    // ind - 15
    if ((knight_bb & BitBoard::kHFileBitboard) == 0
        && (knight_bb & (BitBoard::k2RankBitboard | BitBoard::k1RankBitboard)) == 0) {

        attack_pattern.push_back(knight_pos - 15);
    }

    return attack_pattern;
}

