//
// Created by Kostya on 9/2/2023.
//

#include "MoveGenerator.h"
#include "BoardRayIterator.h"
#include "PrecomputedPiecePatterns.h"

std::vector<Move> MoveGenerator::GenerateMoves(const BitBoard& board, PlayerColor player) {
    std::vector<Move> all_moves;
    if (player == PlayerColor::PLAYER_NUMBER) {
        player = board.GetPlayerToMove();
    }

    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhitePawn));
    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhiteKnight));
    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhiteBishop));
    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhiteRook));
    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhiteQueen));
    GeneratePieceMoves(board, &all_moves, player, static_cast<PieceType>(player * 6 + WhiteKing));

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

void MoveGenerator::GeneratePawnMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard board_occupancy = GenerateOccupiedPositions(board);

    bitboard pawns = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhitePawn));

    while(pawns) {
        bitboard pos_bb = BoardRayIterator::LS1B(pawns);
        squareInd pos = BoardRayIterator::MS1BInd(pos_bb);

        pawns ^= pos_bb;

        bitboard pushes = PrecomputedPiecePatterns::GetPawnPushPattern(player, pos);

        while(pushes) {
            bitboard push_square_bb =
                    (player == PlayerColor::White ? BoardRayIterator::LS1B(pushes)
                     : BoardRayIterator::MS1B(pushes));
            pushes ^= push_square_bb;

            if ((push_square_bb & board_occupancy) == 0) {
                Move move{};

                move.source_square = pos;
                move.target_square = BoardRayIterator::MS1BInd(push_square_bb);

                move.source_piece = static_cast<PieceType>(player * 6 + WhitePawn);
                move.target_piece = static_cast<PieceType>(player * 6 + WhitePawn);

                if ((pos_bb & (player == PlayerColor::White ? BitBoard::k7RankBitboard : BitBoard::k2RankBitboard)) == 0) {
                    move.type = MoveSimple;
                    all_moves->push_back(move);
                }
                else {
                    move.type = PromotionSimple;

                    for (size_t piece = 1 + player * 6; piece < player * 6 + WhiteKing; ++piece) {
                        move.promotion_piece = static_cast<PieceType>(piece);

                        all_moves->push_back(move);
                    }
                }
            }
            else {
                break;
            }
        }

        bitboard attacks = PrecomputedPiecePatterns::GetPieceAttackPattern(
                player, static_cast<PieceType>(player * 6 + WhitePawn), pos, 0ull);
        while(attacks) {
            bitboard attack_square_bb = BoardRayIterator::LS1B(attacks);
            attacks ^= attack_square_bb;

            if ((attack_square_bb & opponent_occupancy) != 0) {
                Move move{};

                move.source_square = pos;
                move.target_square = BoardRayIterator::MS1BInd(attack_square_bb);

                move.type = CaptureSimple;

                move.source_piece = static_cast<PieceType>(player * 6 + WhitePawn);
                move.target_piece = board.GetPieceTypeBySquare(attack_square_bb);

                if ((pos_bb & (player == PlayerColor::White ? BitBoard::k7RankBitboard : BitBoard::k2RankBitboard)) == 0) {
                    move.type = CaptureSimple;
                    all_moves->push_back(move);
                }
                else {
                    move.type = CapturePromotion;

                    for (size_t piece = 1 + player * 6; piece < player * 6 + WhiteKing; ++piece) {
                        move.promotion_piece = static_cast<PieceType>(piece);

                        all_moves->push_back(move);
                    }
                }
            }
            else if (board.GetEnPassantAttackSquare() != BitBoard::kNoEnPassantFlag &&
                    (attack_square_bb & (1ull << board.GetEnPassantAttackSquare())) != 0) {

                Move move{};

                move.source_square = pos;
                move.target_square = BoardRayIterator::MS1BInd(attack_square_bb);

                move.type = CaptureEnPassant;

                move.source_piece = static_cast<PieceType>(player * 6 + WhitePawn);
                move.target_piece = static_cast<PieceType>(!player * 6 + WhitePawn);

                move.promotion_piece = WhitePawn;

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GeneratePieceMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player, PieceType piece) {
    if (piece == WhitePawn || piece == BlackPawn) {
        GeneratePawnMoves(board, all_moves, player);
        return;
    }
    if (piece == WhiteKing || piece == BlackKing) {
        GenerateCastleMoves(board, all_moves, player);
    }

    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard piece_bb = board.GetPiecePositions(piece);
    while(piece_bb) {
        squareInd piece_square = BoardRayIterator::MS1BInd(piece_bb);
        piece_bb ^= (1ull << piece_square);

        bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetPieceAttackPattern(player, piece, piece_square, ally_occupancy |
                                                                                                   opponent_occupancy);
        attack_pattern_bb &= ~ally_occupancy;
        while(attack_pattern_bb) {
            squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
            attack_pattern_bb ^= (1ull << attack_square);

            if (((1ull << attack_square) & ally_occupancy) != 0) {
                continue;
            }

            Move move{};
            move.source_square = piece_square;
            move.target_square = attack_square;

            move.source_piece = piece;

            // capturing
            if (((1ull << attack_square) & opponent_occupancy) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = piece;

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateCastleMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard king_bb = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteKing));
    if (king_bb == 0) {
        return;
    }
    squareInd king_pos_ind = BoardRayIterator::MS1BInd(king_bb);

    // long castling
    bitboard opponent_attacks = 0ull;
    if ((player == PlayerColor::White && board.IsWhiteLongCastleAllowed()
        || player == PlayerColor::Black && board.IsBlackLongCastleAllowed())
        && ((opponent_occupancy | ally_occupancy) & ((king_bb >> 1) | (king_bb >> 2) | (king_bb >> 3))) == 0) {

        opponent_attacks = GeneratePlayerAttacks(board, (player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White));
        if ((opponent_attacks & ((king_bb >> 1) | (king_bb >> 2) | king_bb)) == 0) {
            Move move{};

            move.source_square = king_pos_ind;
            move.target_square = king_pos_ind - 2;

            move.type = CastlingLong;

            move.source_piece = static_cast<PieceType>(player * 6 + WhiteKing);
            move.target_piece = static_cast<PieceType>(player * 6 + WhiteKing);

            move.promotion_piece = static_cast<PieceType>(player * 6 + WhiteKing);

            all_moves->push_back(move);
        }
    }

    // short castling
    if ((player == PlayerColor::White && board.IsWhiteShortCastleAllowed()
         || player == PlayerColor::Black && board.IsBlackShortCastleAllowed())
        && ((opponent_occupancy | ally_occupancy) & ((king_bb << 1) | (king_bb << 2))) == 0) {

        if (opponent_attacks == 0) {
            opponent_attacks = GeneratePlayerAttacks(board, (player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White));
        }
        if ((opponent_attacks & ((king_bb << 1) | (king_bb << 2) | king_bb)) == 0) {
            Move move{};

            move.source_square = king_pos_ind;
            move.target_square = king_pos_ind + 2;

            move.type = CastlingShort;

            move.source_piece = static_cast<PieceType>(player * 6 + WhiteKing);
            move.target_piece = static_cast<PieceType>(player * 6 + WhiteKing);

            move.promotion_piece = static_cast<PieceType>(player * 6 + WhiteKing);

            all_moves->push_back(move);
        }
    }
}

bitboard MoveGenerator::GeneratePlayerAttacks(const BitBoard& board, PlayerColor player) {
    bitboard attacks_bb = 0;
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));
    bitboard blockers = opponent_occupancy | ally_occupancy;

    for (size_t piece_ind = static_cast<PieceType>(player * 6 + WhitePawn); piece_ind <= player * 6 + WhiteKing; ++piece_ind) {
        bitboard pieces = board.GetPiecePositions(static_cast<PieceType>(piece_ind));
        while(pieces) {
            bitboard pos_bb = BoardRayIterator::LS1B(pieces);
            squareInd pos = BoardRayIterator::MS1BInd(pos_bb);

            pieces ^= pos_bb;

            bitboard pawn_attacks_bb = PrecomputedPiecePatterns::GetPieceAttackPattern(
                    player, static_cast<PieceType>(piece_ind), pos, blockers);
            attacks_bb |= pawn_attacks_bb;
        }
    }

    attacks_bb &= ~ally_occupancy;
    return attacks_bb;
}

bool MoveGenerator::IsKingInCheck(const BitBoard &board, PlayerColor player) {
    if (player == PLAYER_NUMBER) {
        player = board.GetPlayerToMove();
    }

    bitboard opponent_attacks =
            MoveGenerator::GeneratePlayerAttacks(board,
                                                 player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);
    return (opponent_attacks & board.GetPiecePositions(static_cast<PieceType>(WhiteKing + 6 * player))) != 0;
}
