//
// Created by Kostya on 9/2/2023.
//

#ifndef LCOS10GINE_MOVEGENERATOR_H
#define LCOS10GINE_MOVEGENERATOR_H

#include <vector>
#include <array>

#include "BoardRepresentation/BitBoard.h"

class BitBoard;

enum MoveType : std::uint8_t {
    MoveSimple,

    CaptureSimple,
    CaptureEnPassant,
    CapturePromotion,
    PromotionSimple,

    CastlingLeft,
    CastlingRight,

    MOVE_TYPE_LEN
};

struct Move {
    Move() = default;

    std::uint8_t source_square;
    std::uint8_t target_square;

    MoveType type;

    PieceType source_piece;
    PieceType target_piece;

    PieceType promotion_piece;
};

class MoveGenerator {
public:

    static std::vector<Move> GenerateMoves(const BitBoard& board);

private:

    static bitboard GenerateOccupiedPositions(const BitBoard &board);
    static bitboard GenerateBlackOccupiedPositions(const BitBoard &board);
    static bitboard GenerateWhiteOccupiedPositions(const BitBoard &board);

    static void GenerateWhitePawnMoves(const BitBoard &board, std::vector<Move> *all_moves);
    static void GenerateWhiteKnightMoves(const BitBoard &board, std::vector<Move> *all_moves);

    static void GenerateBlackPawnMoves(const BitBoard &board, std::vector<Move> *all_moves);
    static void GenerateBlackKnightMoves(const BitBoard &board, std::vector<Move> *all_moves);

    static std::vector<std::uint8_t> GenerateKnightAttackPattern(std::uint8_t knight_pos);

    constexpr static const std::array<bitboard, 64> kStraightAttackPattern
    {
            72340172838076926ull,
            144680345676153597ull,
            289360691352306939ull,
            578721382704613623ull,
            1157442765409226991ull,
            2314885530818453727ull,
            4629771061636907199ull,
            9259542123273814143ull,
            72340172838141441ull,
            144680345676217602ull,
            289360691352369924ull,
            578721382704674568ull,
            1157442765409283856ull,
            2314885530818502432ull,
            4629771061636939584ull,
            9259542123273813888ull,
            72340172854657281ull,
            144680345692602882ull,
            289360691368494084ull,
            578721382720276488ull,
            1157442765423841296ull,
            2314885530830970912ull,
            4629771061645230144ull,
            9259542123273748608ull,
            72340177082712321ull,
            144680349887234562ull,
            289360695496279044ull,
            578721386714368008ull,
            1157442769150545936ull,
            2314885534022901792ull,
            4629771063767613504ull,
            9259542123257036928ull,
            72341259464802561ull,
            144681423712944642ull,
            289361752209228804ull,
            578722409201797128ull,
            1157443723186933776ull,
            2314886351157207072ull,
            4629771607097753664ull,
            9259542118978846848ull,
            72618349279904001ull,
            144956323094725122ull,
            289632270724367364ull,
            578984165983651848ull,
            1157687956502220816ull,
            2315095537539358752ull,
            4629910699613634624ull,
            9259541023762186368ull,
            143553341945872641ull,
            215330564830528002ull,
            358885010599838724ull,
            645993902138460168ull,
            1220211685215703056ull,
            2368647251370188832ull,
            4665518383679160384ull,
            9259260648297103488ull,
            18302911464433844481ull,
            18231136449196065282ull,
            18087586418720506884ull,
            17800486357769390088ull,
            17226286235867156496ull,
            16077885992062689312ull,
            13781085504453754944ull,
            9187484529235886208ull,
    };

    constexpr static const std::array<bitboard, 64> kSlidingAttackPattern
    {
            9241421688590303744ull,
            36099303471056128ull,
            141012904249856ull,
            550848566272ull,
            6480472064ull,
            1108177604608ull,
            283691315142656ull,
            72624976668147712ull,
            4620710844295151618ull,
            9241421688590368773ull,
            36099303487963146ull,
            141017232965652ull,
            1659000848424ull,
            283693466779728ull,
            72624976676520096ull,
            145249953336262720ull,
            2310355422147510788ull,
            4620710844311799048ull,
            9241421692918565393ull,
            36100411639206946ull,
            424704217196612ull,
            72625527495610504ull,
            145249955479592976ull,
            290499906664153120ull,
            1155177711057110024ull,
            2310355426409252880ull,
            4620711952330133792ull,
            9241705379636978241ull,
            108724279602332802ull,
            145390965166737412ull,
            290500455356698632ull,
            580999811184992272ull,
            577588851267340304ull,
            1155178802063085600ull,
            2310639079102947392ull,
            4693335752243822976ull,
            9386671504487645697ull,
            326598935265674242ull,
            581140276476643332ull,
            1161999073681608712ull,
            288793334762704928ull,
            577868148797087808ull,
            1227793891648880768ull,
            2455587783297826816ull,
            4911175566595588352ull,
            9822351133174399489ull,
            1197958188344280066ull,
            2323857683139004420ull,
            144117404414255168ull,
            360293502378066048ull,
            720587009051099136ull,
            1441174018118909952ull,
            2882348036221108224ull,
            5764696068147249408ull,
            11529391036782871041ull,
            4611756524879479810ull,
            567382630219904ull,
            1416240237150208ull,
            2833579985862656ull,
            5667164249915392ull,
            11334324221640704ull,
            22667548931719168ull,
            45053622886727936ull,
            18049651735527937ull,
    };

};


#endif //LCOS10GINE_MOVEGENERATOR_H
