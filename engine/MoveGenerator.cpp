//
// Created by Kostya on 9/2/2023.
//

#include "MoveGenerator.h"
#include "BoardRayIterator.h"

std::vector<std::vector<bitboard>> MoveGenerator::kPawnPushesPattern;
std::vector<std::vector<bitboard>> MoveGenerator::kPawnAttacksPattern;

std::vector<Move> MoveGenerator::GenerateMoves(const BitBoard& board, PlayerColor player) {
    std::vector<Move> all_moves;

    if (player == PlayerColor::PLAYER_NUMBER && board.GetPlayerToMove() == PlayerColor::White
        || player == PlayerColor::White) {
        GeneratePawnMoves(board, &all_moves, PlayerColor::White);
        GenerateWhiteKnightMoves(board, &all_moves);
        GenerateBishopMoves(board, &all_moves, PlayerColor::White);
        GenerateRookMoves(board, &all_moves, PlayerColor::White);
        GenerateQueenMoves(board, &all_moves, PlayerColor::White);
        GenerateKingMoves(board, &all_moves, PlayerColor::White);
    }
    else {
        GeneratePawnMoves(board, &all_moves, PlayerColor::Black);
        GenerateBlackKnightMoves(board, &all_moves);
        GenerateBishopMoves(board, &all_moves, PlayerColor::Black);
        GenerateRookMoves(board, &all_moves, PlayerColor::Black);
        GenerateQueenMoves(board, &all_moves, PlayerColor::Black);
        GenerateKingMoves(board, &all_moves, PlayerColor::Black);
    }

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
    GeneratePawnPrecomputePatterns();

    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard board_occupancy = GenerateOccupiedPositions(board);

    bitboard pawns = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhitePawn));

    while(pawns) {
        bitboard pos_bb = BoardRayIterator::LS1B(pawns);
        squareInd pos = BoardRayIterator::MS1BInd(pos_bb);

        pawns ^= pos_bb;

        bitboard pushes = kPawnPushesPattern[player][pos];

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

        bitboard attacks = kPawnAttacksPattern[player][pos];
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
            else if ((attack_square_bb & (1ull << board.GetEnPassantAttackSquare())) != 0) {
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

void MoveGenerator::GenerateWhiteKnightMoves(const BitBoard &board, std::vector<Move> *all_moves) {
    bitboard white_occupied_positions = GenerateWhiteOccupiedPositions(board);
    bitboard black_occupied_positions = GenerateBlackOccupiedPositions(board);

    for (std::uint8_t ind = 0; ind < 64; ++ind) {
        if (((board.GetPiecePositions(PieceType::WhiteKnight) >> ind) & 1) == 0) {
            continue;
        }

        auto attack_pattern = GenerateKnightAttackPattern(ind);

        for (const auto attack_square : attack_pattern) {
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

void MoveGenerator::GenerateBishopMoves(const BitBoard &board, std::vector<Move> *all_moves, const PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
            GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
            GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    std::vector<bitboard> bishops = BoardRayIterator::Bitscan(
            board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteBishop)));

    for (const auto pos : bishops) {
        BoardRayIterator it;
        int dir = RayDirection::NE;
        while(dir < RayDirection::NUMBER_OF_DIRECTIONS) {
            it.SetBoardRay(pos, static_cast<RayDirection>(dir));
            while(bitboard next_attack = it.GetNextRaySquareBitboard()) {
                // empty square
                if ((next_attack & (opponent_occupancy | ally_occupancy)) == 0) {
                    Move move{};

                    move.source_square = BoardRayIterator::MS1BInd(pos);
                    move.target_square = BoardRayIterator::MS1BInd(next_attack);

                    move.type = MoveSimple;

                    move.source_piece = static_cast<PieceType>(player * 6 + WhiteBishop);
                    move.target_piece = static_cast<PieceType>(player * 6 + WhiteBishop);

                    all_moves->push_back(move);
                }
                // attack
                else if ((next_attack & opponent_occupancy) != 0) {
                    Move move{};

                    move.source_square = BoardRayIterator::MS1BInd(pos);
                    move.target_square = BoardRayIterator::MS1BInd(next_attack);

                    move.type = CaptureSimple;

                    move.source_piece = static_cast<PieceType>(player * 6 + WhiteBishop);
                    move.target_piece = board.GetPieceTypeBySquare(next_attack);
                    all_moves->push_back(move);
                    break;
                }
                // ally blocker
                else if ((next_attack & ally_occupancy) != 0) {
                    break;
                }
            }
            dir += 2;
        }
    }
}

void MoveGenerator::GenerateRookMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    std::vector<bitboard> rooks = BoardRayIterator::Bitscan(
            board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteRook)));

    for (const auto pos : rooks) {
        BoardRayIterator it;
        int dir = RayDirection::E;
        while(dir < RayDirection::NUMBER_OF_DIRECTIONS) {
            it.SetBoardRay(pos, static_cast<RayDirection>(dir));
            while(bitboard next_attack = it.GetNextRaySquareBitboard()) {
                // empty square
                if ((next_attack & (opponent_occupancy | ally_occupancy)) == 0) {
                    Move move{};

                    move.source_square = BoardRayIterator::MS1BInd(pos);
                    move.target_square = BoardRayIterator::MS1BInd(next_attack);

                    move.type = MoveSimple;

                    move.source_piece = static_cast<PieceType>(player * 6 + WhiteRook);
                    move.target_piece = static_cast<PieceType>(player * 6 + WhiteRook);

                    all_moves->push_back(move);
                }
                    // attack
                else if ((next_attack & opponent_occupancy) != 0) {
                    Move move{};

                    move.source_square = BoardRayIterator::MS1BInd(pos);
                    move.target_square = BoardRayIterator::MS1BInd(next_attack);

                    move.type = CaptureSimple;

                    move.source_piece = static_cast<PieceType>(player * 6 + WhiteRook);
                    move.target_piece = board.GetPieceTypeBySquare(next_attack);
                    all_moves->push_back(move);
                    break;
                }
                    // ally blocker
                else if ((next_attack & ally_occupancy) != 0) {
                    break;
                }
            }
            dir += 2;
        }
    }
}

void MoveGenerator::GenerateQueenMoves(const BitBoard &board, std::vector<Move> *all_moves, PlayerColor player) {
    bitboard opponent_occupancy = (player == PlayerColor::White ?
                                   GenerateBlackOccupiedPositions(board) : GenerateWhiteOccupiedPositions(board));
    bitboard ally_occupancy = (player == PlayerColor::White ?
                               GenerateWhiteOccupiedPositions(board) : GenerateBlackOccupiedPositions(board));

    bitboard queen_pos = board.GetPiecePositions(static_cast<PieceType>(player * 6 + WhiteQueen));
    if (queen_pos == 0) {
        return;
    }

    BoardRayIterator it;
    int dir = RayDirection::E;
    while(dir < RayDirection::NUMBER_OF_DIRECTIONS) {
        it.SetBoardRay(queen_pos, static_cast<RayDirection>(dir));
        while(bitboard next_attack = it.GetNextRaySquareBitboard()) {
            // empty square
            if ((next_attack & (opponent_occupancy | ally_occupancy)) == 0) {
                Move move{};

                move.source_square = BoardRayIterator::MS1BInd(queen_pos);
                move.target_square = BoardRayIterator::MS1BInd(next_attack);

                move.type = MoveSimple;

                move.source_piece = static_cast<PieceType>(player * 6 + WhiteQueen);
                move.target_piece = static_cast<PieceType>(player * 6 + WhiteQueen);

                all_moves->push_back(move);
            }
                // attack
            else if ((next_attack & opponent_occupancy) != 0) {
                Move move{};

                move.source_square = BoardRayIterator::MS1BInd(queen_pos);
                move.target_square = BoardRayIterator::MS1BInd(next_attack);

                move.type = CaptureSimple;

                move.source_piece = static_cast<PieceType>(player * 6 + WhiteQueen);
                move.target_piece = board.GetPieceTypeBySquare(next_attack);
                all_moves->push_back(move);
                break;
            }
                // ally blocker
            else if ((next_attack & ally_occupancy) != 0) {
                break;
            }
        }
        ++dir;
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

    auto attack_pattern = GenerateKingAttackPattern(king_pos_ind);

    for (const auto attack_square : attack_pattern) {
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
    if ((player == PlayerColor::White && board.IsWhiteLongCastleAllowed()
        || player == PlayerColor::Black && board.IsBlackLongCastleAllowed())
        && ((opponent_occupancy | ally_occupancy) & ((king_bb >> 1) | (king_bb >> 2) | (king_bb >> 3))) == 0) {

        Move move{};

        move.source_square = king_pos_ind;
        move.target_square = king_pos_ind - 2;

        move.type = CastlingLong;

        move.source_piece = static_cast<PieceType>(player * 6 + WhiteKing);
        move.target_piece = static_cast<PieceType>(player * 6 + WhiteKing);

        move.promotion_piece = static_cast<PieceType>(player * 6 + WhiteKing);

        all_moves->push_back(move);
    }

    // short castling
    if ((player == PlayerColor::White && board.IsWhiteShortCastleAllowed()
         || player == PlayerColor::Black && board.IsBlackShortCastleAllowed())
        && ((opponent_occupancy | ally_occupancy) & ((king_bb << 1) | (king_bb << 2))) == 0) {

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

std::vector<std::uint8_t> MoveGenerator::GenerateKingAttackPattern(std::uint8_t king_pos) {
    std::vector<std::uint8_t> attack_pattern;
    attack_pattern.reserve(8);

    bitboard king_bb = (1ull << king_pos);

    // ind - 1
    if ((king_bb & BitBoard::kAFileBitboard) == 0) {
        attack_pattern.push_back(king_pos - 1);
    }

    // ind + 1
    if ((king_bb & BitBoard::kHFileBitboard) == 0) {
        attack_pattern.push_back(king_pos + 1);
    }

    // ind - 8
    if ((king_bb & BitBoard::k1RankBitboard) == 0) {
        attack_pattern.push_back(king_pos - 8);
    }

    // ind + 8
    if ((king_bb & BitBoard::k8RankBitboard) == 0) {
        attack_pattern.push_back(king_pos + 8);
    }

    // ind + 7
    if ((king_bb & BitBoard::kAFileBitboard) == 0
        && (king_bb & BitBoard::k8RankBitboard) == 0) {

        attack_pattern.push_back(king_pos + 7);
    }

    // ind + 9
    if ((king_bb & BitBoard::kHFileBitboard) == 0
        && (king_bb & BitBoard::k8RankBitboard) == 0) {

        attack_pattern.push_back(king_pos + 9);
    }

    // ind - 9
    if ((king_bb & BitBoard::kAFileBitboard) == 0
        && (king_bb & BitBoard::k1RankBitboard) == 0) {

        attack_pattern.push_back(king_pos - 9);
    }

    // ind - 7
    if ((king_bb & BitBoard::kHFileBitboard) == 0
        && (king_bb & BitBoard::k1RankBitboard) == 0) {

        attack_pattern.push_back(king_pos - 7);
    }

    return attack_pattern;
}

bitboard MoveGenerator::GeneratePlayerAttacks(const BitBoard& board, PlayerColor player) {
    std::vector<Move> moves = GenerateMoves(board, player);

    bitboard attacks_bb = 0;
    for (const auto &move : moves) {
        attacks_bb |= (1ull << move.target_square);
    }

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

void MoveGenerator::GeneratePawnPrecomputePatterns() {
    if (kPawnPushesPattern.size() == 2) {
        return;
    }

    kPawnPushesPattern.assign(PLAYER_NUMBER, std::vector<bitboard>(64, 0));
    kPawnAttacksPattern.assign(PLAYER_NUMBER, std::vector<bitboard>(64, 0));

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);

        bitboard pushes_bb = 0;
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k8RankBitboard) == 0) {
            pushes_bb |= (square_bb << 8);
        }
        if ((square_bb & BitBoard::k2RankBitboard) != 0) {
            pushes_bb |= (square_bb << 16);
        }

        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb << 7);
        }
        if ((square_bb & BitBoard::k8RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb << 9);
        }

        kPawnPushesPattern[PlayerColor::White][square] = pushes_bb;
        kPawnAttacksPattern[PlayerColor::White][square] = attacks_bb;
    }

    for (squareInd square = 0; square < 64; ++square) {
        bitboard square_bb = (1ull << square);

        bitboard pushes_bb = 0;
        bitboard attacks_bb = 0;

        if ((square_bb & BitBoard::k1RankBitboard) == 0) {
            pushes_bb |= (square_bb >> 8);
        }
        if ((square_bb & BitBoard::k7RankBitboard) != 0) {
            pushes_bb |= (square_bb >> 16);
        }

        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kAFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 9);
        }
        if ((square_bb & BitBoard::k1RankBitboard) == 0
            && (square_bb & BitBoard::kHFileBitboard) == 0) {
            attacks_bb |= (square_bb >> 7);
        }

        kPawnPushesPattern[PlayerColor::Black][square] = pushes_bb;
        kPawnAttacksPattern[PlayerColor::Black][square] = attacks_bb;
    }
}