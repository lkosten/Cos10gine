//
// Created by Kostya on 9/2/2023.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::GenerateMoves(const BitBoard& board) {
    std::vector<Move> all_moves;

    GenerateWhitePawnMoves(board, &all_moves);

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

                for (size_t piece = 2; piece < BlackPawn; ++piece) {
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

                for (size_t piece = 2; piece < BlackPawn; ++piece) {
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

                for (size_t piece = 2; piece < BlackPawn; ++piece) {
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
