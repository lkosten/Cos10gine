//
// Created by Kostya on 1/11/2024.
//

#ifndef LCOS10GINE_UCI_H
#define LCOS10GINE_UCI_H


#include <iostream>

#include "GrandMaster.h"

class UCI {
public:
    UCI();
    explicit UCI(std::ostream &ostream);

    void MainUpdateLoop(std::istream &is);

    std::string PerformUCICommand(const std::string &cmd_line);

private:
    std::ostream &output_stream;

    GrandMaster grand_master;

    static std::string MoveToString(const Move &move);

    void PefrormUCIPosition(std::stringstream &is_args);
    std::string PerformUCIGo(std::stringstream &is_args);
};


#endif //LCOS10GINE_UCI_H