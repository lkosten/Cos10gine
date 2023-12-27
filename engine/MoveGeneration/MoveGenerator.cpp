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

    GeneratePawnMoves(board, &all_moves, player);
    GenerateKnightMoves(board, &all_moves, player);
    GenerateBishopMoves(board, &all_moves, player);
    GenerateRookMoves(board, &all_moves, player);
    GenerateQueenMoves(board, &all_moves, player);
    GenerateKingMoves(board, &all_moves, player);

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

        bitboard attacks = PrecomputedPiecePatterns::GetPawnAttackPattern(player, pos);
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

void MoveGenerator::GenerateKnightMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard ally_occupied_positions = (player == PlayerColor::White ? GenerateWhiteOccupiedPositions(board)
                                        : GenerateBlackOccupiedPositions(board));
    bitboard opponent_occupied_positions = (player == PlayerColor::Black ? GenerateWhiteOccupiedPositions(board)
                                            : GenerateBlackOccupiedPositions(board));

    bitboard knights = board.GetPiecePositions(static_cast<PieceType>(player * 6 + PieceType::WhiteKnight));
    while(knights) {
        squareInd ind = BoardRayIterator::MS1BInd(knights);
        knights ^= (1ull << ind);

        bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetKnightAttackPattern(ind);
        attack_pattern_bb &= ~ally_occupied_positions;
        while(attack_pattern_bb) {
            squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
            attack_pattern_bb ^= (1ull << attack_square);

            if (((1ull << attack_square) & ally_occupied_positions) != 0) {
                continue;
            }

            Move move{};
            move.source_square = ind;
            move.target_square = attack_square;

            move.source_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteKnight);

            // capturing
            if (((1ull << attack_square) & opponent_occupied_positions) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteKnight);

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateBishopMoves(const BitBoard &board, std::vector<Move> *all_moves, const PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
            GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
            GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard bishops = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteBishop));
    while(bishops) {
        squareInd bishop_square = BoardRayIterator::MS1BInd(bishops);
        bishops ^= (1ull << bishop_square);

        bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetBishopAttackPattern(bishop_square, ally_occupancy | opponent_occupancy);
        attack_pattern_bb &= ~ally_occupancy;
        while(attack_pattern_bb) {
            squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
            attack_pattern_bb ^= (1ull << attack_square);

            if (((1ull << attack_square) & ally_occupancy) != 0) {
                continue;
            }

            Move move{};
            move.source_square = bishop_square;
            move.target_square = attack_square;

            move.source_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteBishop);

            // capturing
            if (((1ull << attack_square) & opponent_occupancy) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteBishop);

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateRookMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard bishops = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteRook));
    while(bishops) {
        squareInd bishop_square = BoardRayIterator::MS1BInd(bishops);
        bishops ^= (1ull << bishop_square);

        bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetRookAttackPattern(bishop_square, ally_occupancy | opponent_occupancy);
        attack_pattern_bb &= ~ally_occupancy;
        while(attack_pattern_bb) {
            squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
            attack_pattern_bb ^= (1ull << attack_square);

            if (((1ull << attack_square) & ally_occupancy) != 0) {
                continue;
            }

            Move move{};
            move.source_square = bishop_square;
            move.target_square = attack_square;

            move.source_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteRook);

            // capturing
            if (((1ull << attack_square) & opponent_occupancy) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteRook);

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateQueenMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard bishops = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteQueen));
    while(bishops) {
        squareInd bishop_square = BoardRayIterator::MS1BInd(bishops);
        bishops ^= (1ull << bishop_square);

        bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetQueenAttackPattern(bishop_square, ally_occupancy | opponent_occupancy);
        attack_pattern_bb &= ~ally_occupancy;
        while(attack_pattern_bb) {
            squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
            attack_pattern_bb ^= (1ull << attack_square);

            if (((1ull << attack_square) & ally_occupancy) != 0) {
                continue;
            }

            Move move{};
            move.source_square = bishop_square;
            move.target_square = attack_square;

            move.source_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteQueen);

            // capturing
            if (((1ull << attack_square) & opponent_occupancy) != 0) {
                move.type = CaptureSimple;
                move.target_piece = board.GetPieceTypeBySquare(1ull << attack_square);

                all_moves->push_back(move);
            }
            else { // simple move
                move.type = MoveSimple;
                move.target_piece = static_cast<PieceType>(player * 6 + PieceType::WhiteQueen);

                all_moves->push_back(move);
            }
        }
    }
}

void MoveGenerator::GenerateKingMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard king_bb = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteKing));
    if (king_bb == 0) {
        return;
    }
    squareInd king_pos_ind = BoardRayIterator::MS1BInd(king_bb);

    bitboard attack_pattern_bb = PrecomputedPiecePatterns::GetKingAttackPattern(king_pos_ind);
    while (attack_pattern_bb) {
        squareInd attack_square = BoardRayIterator::MS1BInd(attack_pattern_bb);
        attack_pattern_bb ^= (1ull << attack_square);

        if (((1ull << attack_square) & ally_occupancy) != 0) {
            continue;
        }

        Move move{};
        move.source_square = king_pos_ind;
        move.target_square = attack_square;

        move.source_piece = static_cast<PieceType>(player * 6 + WhiteKing);
        move.promotion_piece = static_cast<PieceType>(player * 6 + WhiteKing);

        // capturing
        if (((1ull << attack_square) & opponent_occupancy) != 0) {
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

    // Pawns
    bitboard pawns = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhitePawn));
    while(pawns) {
        bitboard pos_bb = BoardRayIterator::LS1B(pawns);
        squareInd pos = BoardRayIterator::MS1BInd(pos_bb);

        pawns ^= pos_bb;

        bitboard pawn_attacks_bb = PrecomputedPiecePatterns::GetPawnAttackPattern(player, pos);
        attacks_bb |= pawn_attacks_bb;
    }

    // Knights
    bitboard knights = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteKnight));
    while(knights) {
        bitboard pos_bb = BoardRayIterator::LS1B(knights);
        squareInd pos = BoardRayIterator::MS1BInd(pos_bb);

        knights ^= pos_bb;

        bitboard knight_attacks_bb = PrecomputedPiecePatterns::GetKnightAttackPattern(pos);
        attacks_bb |= knight_attacks_bb;
    }

    // King
    {
        bitboard king = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteKing));
        squareInd pos = BoardRayIterator::MS1BInd(king);

        bitboard king_attacks_bb = PrecomputedPiecePatterns::GetKingAttackPattern(pos);
        attacks_bb |= king_attacks_bb;
    }

    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    // Bishops
    bitboard bishops = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteBishop));
    while(bishops) {
        squareInd pos = BoardRayIterator::MS1BInd(bishops);
        bishops ^= (1ull << pos);

        attacks_bb |= PrecomputedPiecePatterns::GetBishopAttackPattern(pos, opponent_occupancy | ally_occupancy);
    }

    // Rooks
    bitboard rooks = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteRook));
    while (rooks) {
        squareInd pos = BoardRayIterator::MS1BInd(rooks);
        rooks ^= (1ull << pos);

        attacks_bb |= PrecomputedPiecePatterns::GetRookAttackPattern(pos, opponent_occupancy | ally_occupancy);
    }

    // Queens
    bitboard queens = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteQueen));
    while(queens) {
        squareInd pos = BoardRayIterator::MS1BInd(queens);
        queens ^= (1ull << pos);

        attacks_bb |= PrecomputedPiecePatterns::GetQueenAttackPattern(pos, opponent_occupancy | ally_occupancy);
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
