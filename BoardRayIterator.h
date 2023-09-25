//
// Created by Kostya on 9/24/2023.
//

#ifndef LCOS10GINE_BOARDRAYITERATOR_H
#define LCOS10GINE_BOARDRAYITERATOR_H

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

class BoardRayIterator {
public:
    BoardRayIterator();
    BoardRayIterator(squareInd index, RayDirection direction);

    void SetBoardRay(squareInd index, RayDirection direction);
    bitboard GetNextRaySquareBitboard();

private:
    static void GeneratrePrecomputeRays();

    RayDirection ray_direction;
    bitboard current_ray;

    // E -> NE -> N -> ... -> SE
    static std::vector<std::vector<bitboard>> kRayPatterns;

    static bitboard GetRayByDirection(squareInd index, RayDirection direction);

    static bitboard LS1B(bitboard bb);
    static bitboard MS1B(bitboard bb);
};


#endif //LCOS10GINE_BOARDRAYITERATOR_H
