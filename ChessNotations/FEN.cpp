//
// Created by Kostya on 10/4/2023.
//

#include <sstream>

#include "FEN.h"

BitBoard FEN::GetBitBoardFromFEN(const std::string &fen) {
    BitBoard board{};
    std::stringstream sstream(fen);

    std::vector<std::string> tokens;
    std::string cur_token;
    while(std::getline(sstream, cur_token, ' ')) {
        tokens.push_back(cur_token);
    }

    if (tokens.size() != 6) {
        throw std::runtime_error("Wrong FEN format string. Wrong number of tokens:\n" + fen);
    }

    SetPiecePlacement(&board, tokens[0]);
    SetSideToMove(&board, tokens[1]);
    SetCastlingAbility(&board, tokens[2]);
    SetEnPassant(&board, tokens[3]);
    SetHalfMoveClock(&board, tokens[4]);
    SetFullMoveCounter(&board, tokens[5]);

    return board;
}

void FEN::SetPiecePlacement(BitBoard *board, const std::string& piece_placement) {
    std::stringstream sstream(piece_placement);

    std::vector<std::string> tokens;
    std::string cur_token;
    while(std::getline(sstream, cur_token, '/')) {
        tokens.push_back(cur_token);
    }

    if (tokens.size() != 8) {
        throw std::runtime_error("Wrong FEN format string. Wrong piece placement token:\n" + piece_placement);
    }

    std::reverse(tokens.begin(), tokens.end());

    bitboard cur_bb = 1;
    for (const auto &token : tokens) {
        for (const auto item : token) {
            if (item >= '1' && item <= '8') {
                cur_bb <<= (item - '0');
            }
            else {
                PieceType piece;
                switch (item) {
                    case 'P':
                        piece = PieceType::WhitePawn;
                        break;
                    case 'N':
                        piece = PieceType::WhiteKnight;
                        break;
                    case 'B':
                        piece = PieceType::WhiteBishop;
                        break;
                    case 'R':
                        piece = PieceType::WhiteRook;
                        break;
                    case 'Q':
                        piece = PieceType::WhiteQueen;
                        break;
                    case 'K':
                        piece = PieceType::WhiteKing;
                        break;

                    case 'p':
                        piece = PieceType::BlackPawn;
                        break;
                    case 'n':
                        piece = PieceType::BlackKnight;
                        break;
                    case 'b':
                        piece = PieceType::BlackBishop;
                        break;
                    case 'r':
                        piece = PieceType::BlackRook;
                        break;
                    case 'q':
                        piece = PieceType::BlackQueen;
                        break;
                    case 'k':
                        piece = PieceType::BlackKing;
                        break;

                    default:
                        throw std::runtime_error("Wrong FEN format string. Wrong piece placement token:\n" + piece_placement);
                }

                board->f_board[piece] |= cur_bb;
                cur_bb <<= 1;
            }
        }
    }
}

void FEN::SetSideToMove(BitBoard *board, const std::string& side_to_move) {
    if (side_to_move == "w" || side_to_move == "W") {
        board->f_next_turn_player = PlayerColor::White;
    }
    else if (side_to_move == "b" || side_to_move == "B") {
        board->f_next_turn_player = PlayerColor::Black;
    }
    else {
        throw std::runtime_error("Wrong FEN format string. Wrong side to move token:\n" + side_to_move);
    }
}

void FEN::SetCastlingAbility(BitBoard *board, const std::string& castling) {
    board->f_castling_rights.black_short_catle = false;
    board->f_castling_rights.black_long_catle = false;
    board->f_castling_rights.white_short_catle = false;
    board->f_castling_rights.white_long_catle = false;

    for (const auto letter : castling) {
        if (letter == 'k') {
            board->f_castling_rights.black_short_catle = false;
        }
        else if (letter == 'q') {
            board->f_castling_rights.black_long_catle = false;
        }
        else if (letter == 'K') {
            board->f_castling_rights.white_short_catle = false;
        }
        else if (letter == 'Q') {
            board->f_castling_rights.white_long_catle = false;
        }
        else if (letter != '-') {
            throw std::runtime_error("Wrong FEN format string. Wrong castling ability token:\n" + castling);
        }
    }
}

void FEN::SetEnPassant(BitBoard *board, const std::string &en_passant) {
    if (en_passant == "-") {
        board->f_en_passant_square = BitBoard::kNoEnPassantFlag;
        return;
    }

    if (en_passant.size() != 2 || !(en_passant[0] - 'a' >= 0 && en_passant[0] - 'h' <= 0)
        || !(en_passant[1] == '3' || en_passant[1] == '6')) {
        throw std::runtime_error("Wrong FEN format string. Wrong en passant token:\n" + en_passant);
    }

    board->f_en_passant_square = (en_passant[0] - 'a') + 8 * (en_passant[1] - '1');
}

void FEN::SetHalfMoveClock(BitBoard *board, std::string half_moves) {

}

void FEN::SetFullMoveCounter(BitBoard *board, std::string full_moves) {

}
