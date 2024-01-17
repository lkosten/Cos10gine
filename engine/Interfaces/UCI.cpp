//
// Created by Kostya on 1/11/2024.
//

#include <sstream>
#include <thread>

#include "UCI.h"
#include "Search/AlphaBetaSearch.h"

UCI::UCI() : output_stream(std::cout) { }

UCI::UCI(std::ostream &ostream) : output_stream(ostream) { }

void UCI::MainUpdateLoop(std::istream &is) {
    std::string cmd_line;

    std::string response;
    do {
        output_stream << response;
        output_stream.flush();

        std::getline(is, cmd_line);

        response = PerformUCICommand(cmd_line);

    } while (is.eof() || response != "quit");
}

std::string UCI::PerformUCICommand(const std::string &cmd_line) {
    std::stringstream is(cmd_line);
    std::string token;
    is >> token;

    std::string response;
    if (token == "uci") {
        response += "id name lCos10gine\n";
        response += "id author Kanstantsin Yarmash\n";
        response += "uciok\n";
    }
    else if (token == "debug") {
        //
    }
    else if (token == "isready") {
        response += "readyok\n";
    }
    else if (token == "setoption") {
        //
    }
    else if (token == "register") {
        //
    }
    else if (token == "ucinewgame") {
        grand_master.CreateNewGame();
    }
    else if (token == "position") {
        PefrormUCIPosition(is);
    }
    else if (token == "go") {
        PerformUCIGo(is);
    }
    else if (token == "stop" || token == "quit") {
        AlphaBetaSearch::ShouldStopSearch();
        while(search_in_progress.test());

        response = token == "quit" ? "quit" : "";
    }
    else if (token == "ponderhit") {
        //
    }
    else if (token == "register") {
        //
    }

    return response;
}

std::string UCI::MoveToString(const Move &move) {
    std::string result = "a1a1";

    result[0] += move.source_square & 7u;
    result[1] += move.source_square >> 3u;

    result[2] += move.target_square & 7u;
    result[3] += move.target_square >> 3u;

    if (move.type == MoveType::CapturePromotion || move.type == PromotionSimple) {
        switch (move.promotion_piece) {
            case WhiteKnight:
            case BlackKnight:
                result += 'n';
                break;

            case WhiteBishop:
            case BlackBishop:
                result += 'b';
                break;

            case WhiteRook:
            case BlackRook:
                result += 'r';
                break;

            case WhiteQueen:
            case BlackQueen:
                result += 'q';
                break;

            default:
                break;
        }
    }

    return result;
}


void UCI::PefrormUCIPosition(std::stringstream &is_args) {
    std::string token;
    is_args >> token;

    if (token == "fen") {
        std::string fen;

        while (is_args >> token && token != "moves") {
            fen += token + " ";
        }
    }

    if (token == "startpos") {
        grand_master.CreateNewGame();

        is_args >> token;
    }

    if (token == "moves") {
        while(is_args >> token) {
            if (token.size() == 5) {
                token[4] = tolower(token[4]);
            }
            auto moves = MoveGenerator::GenerateMoves(grand_master.GetCurrentBoard());

            for (const auto &move: moves) {
                if (MoveToString(move) == token) {
                    if (!grand_master.TryMakeMove(move)) {
                        throw std::runtime_error("Wrong move from uci command: " + token + "\n");
                    }
                    break;
                }
            }
        }
    }

}

void UCI::PerformUCIGo(std::stringstream &is_args) {
    // read is_args


    std::thread search_thread(&UCI::FindBestMove, this);
    search_thread.detach();
}

void UCI::FindBestMove() {
    while(search_in_progress.test_and_set());

    output_stream << "bestmove " + MoveToString(grand_master.GetBestMove()) + "\n";
    output_stream.flush();

    search_in_progress.clear();
}
