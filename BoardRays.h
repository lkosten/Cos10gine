//
// Created by Kostya on 9/24/2023.
//

#ifndef LCOS10GINE_BOARDRAYS_H
#define LCOS10GINE_BOARDRAYS_H

#include <array>

#include "BoardRepresentation/BitBoard.h"

enum RayDirection : std::uint8_t {
    E = 0,
    NE,
    N,
    NW,
    W,
    WS,
    S,
    SE,

    NUMBER_OF_DIRECTIONS
};

class BoardRays {
public:
    BoardRays();

    static bitboard GetRayDirection(squareInd index, RayDirection direction);

private:
    static void GeneratrePrecomputeRays();

    // E -> NE -> N -> ... -> SE
    static std::vector<std::vector<bitboard>> kRayPatterns;
};


#endif //LCOS10GINE_BOARDRAYS_H
