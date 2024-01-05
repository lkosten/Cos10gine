//
// Created by Kostya on 1/5/2024.
//

#ifndef LCOS10GINE_ZOBRISTHASH_H
#define LCOS10GINE_ZOBRISTHASH_H

#include "BitBoard.h"

class Move;

class ZobristHash {
public:
    ZobristHash(const ZobristHash &) = delete;

    ZobristHash(const ZobristHash &&) = delete;

    static ZobristHash &GetInstance() {
        static ZobristHash instance;
        return instance;
    }

    uint64_t GetBoardHash(const BitBoard &board);
    uint64_t GetMoveChangedZobristHash(const BitBoard &board, const Move &move);

private:
    ZobristHash();
    ~ZobristHash() = default;


    uint64_t pieces_hash[PieceType::PIECE_TYPE_LEN][64]{};
    uint64_t black_to_move_hash;
    uint64_t castling_rights_hash[4]{};
    uint64_t valid_en_passant_file_hash[8]{};
};


#endif //LCOS10GINE_ZOBRISTHASH_H
