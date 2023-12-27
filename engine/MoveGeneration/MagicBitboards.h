//
// Created by Kostya on 12/27/2023.
//

#ifndef LCOS10GINE_MAGICBITBOARDS_H
#define LCOS10GINE_MAGICBITBOARDS_H

#include <vector>

#include "BoardRepresentation/BitBoard.h"

class MagicBitboards {

private:
    static std::vector<std::vector<bitboard>> GenerateMagicBitboards(PieceType piece);
    static std::vector<bitboard> GenerateBlockerBitboards(PieceType piece);

    static std::vector<std::vector<bitboard>> kBishopMagicBitboards;
    static std::vector<uint64_t> kBishopMagicNumbers;
    static std::vector<size_t> kBishopMagicNumberFreedomDegrees;
    static std::vector<bitboard> kBishopBlockerBitboards;

    static std::vector<std::vector<bitboard>> kRookMagicBitboards;
    static std::vector<uint64_t> kRookMagicNumbers;
    static std::vector<size_t> kRookMagicNumberFreedomDegrees;
    static std::vector<bitboard> kRookBlockerBitboards;

    static const size_t kFreedomDegreeSupplement;
};


#endif //LCOS10GINE_MAGICBITBOARDS_H
