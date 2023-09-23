//
// Created by Kostya on 9/24/2023.
//

#include "BoardRays.h"

bitboard BoardRays::GetRayDirection(const squareInd index, const RayDirection direction) {
    return kRayPatterns[index][direction];
}
