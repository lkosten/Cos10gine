//
// Created by Kostya on 1/11/2024.
//

#ifndef LCOS10GINE_UCI_H
#define LCOS10GINE_UCI_H


#include <iostream>
#include <atomic>

#include "GrandMaster.h"

struct SearchLimits {
    // player time left in ms
    uint64_t player_time[2];

    // increase of player's time after every move in ms
    uint64_t player_increase[2];

    // moves until next time control
    uint32_t moves_to_go;

    // search positions until specified ply
    uint32_t depth;

    // search only specified number of nodes
    uint64_t nodes;

    // search for mate in specified number of moves
    uint32_t mate;

    // search positions for specified time
    uint64_t move_time;

    // search position until "stop" UCI command
    bool infinite_search;
};

class UCI {
public:
    UCI();
    explicit UCI(std::ostream &ostream);

    void MainUpdateLoop(std::istream &is);

    std::string PerformUCICommand(const std::string &cmd_line);

private:
    std::ostream &output_stream;

    GrandMaster grand_master;
    std::atomic_flag search_in_progress;

    static std::string MoveToString(const Move &move);

    void PefrormUCIPosition(std::stringstream &is_args);
    void PerformUCIGo(std::stringstream &is_args);

    void FindBestMove(SearchLimits limits);
};


#endif //LCOS10GINE_UCI_H
