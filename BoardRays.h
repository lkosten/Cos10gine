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
    static bitboard GetRayDirection(squareInd index, RayDirection direction);

private:
    // E -> NE -> N -> ... -> SE
    constexpr static const bitboard kRayPatterns[64][8] =
            {
                    { 254ull, 9241421688590303744ull, 72340172838076672ull, 0ull, 0ull, 0ull, 0ull, 0ull, },
                    { 252ull, 36099303471055872ull, 144680345676153344ull, 256ull, 1ull, 0ull, 0ull, 0ull, },
                    { 248ull, 141012904183808ull, 289360691352306688ull, 66048ull, 3ull, 0ull, 0ull, 0ull, },
                    { 240ull, 550831656960ull, 578721382704613376ull, 16909312ull, 7ull, 0ull, 0ull, 0ull, },
                    { 224ull, 2151686144ull, 1157442765409226752ull, 4328785920ull, 15ull, 0ull, 0ull, 0ull, },
                    { 192ull, 8404992ull, 2314885530818453504ull, 1108169199616ull, 31ull, 0ull, 0ull, 0ull, },
                    { 128ull, 32768ull, 4629771061636907008ull, 283691315109888ull, 63ull, 0ull, 0ull, 0ull, },
                    { 0ull, 0ull, 9259542123273814016ull, 72624976668147712ull, 127ull, 0ull, 0ull, 0ull, },
                    { 65024ull, 4620710844295151616ull, 72340172838076416ull, 0ull, 0ull, 0ull, 1ull, 2ull, },
                    { 64512ull, 9241421688590303232ull, 144680345676152832ull, 65536ull, 256ull, 1ull, 2ull, 4ull, },
                    { 63488ull, 36099303471054848ull, 289360691352305664ull, 16908288ull, 768ull, 2ull, 4ull, 8ull, },
                    { 61440ull, 141012904181760ull, 578721382704611328ull, 4328783872ull, 1792ull, 4ull, 8ull, 16ull, },
                    { 57344ull, 550831652864ull, 1157442765409222656ull, 1108169195520ull, 3840ull, 8ull, 16ull, 32ull, },
                    { 49152ull, 2151677952ull, 2314885530818445312ull, 283691315101696ull, 7936ull, 16ull, 32ull, 64ull, },
                    { 32768ull, 8388608ull, 4629771061636890624ull, 72624976668131328ull, 16128ull, 32ull, 64ull, 128ull, },
                    { 0ull, 0ull, 9259542123273781248ull, 145249953336262656ull, 32512ull, 64ull, 128ull, 0ull, },
                    { 16646144ull, 2310355422147510272ull, 72340172838010880ull, 0ull, 0ull, 0ull, 257ull, 516ull, },
                    { 16515072ull, 4620710844295020544ull, 144680345676021760ull, 16777216ull, 65536ull, 256ull, 514ull, 1032ull, },
                    { 16252928ull, 9241421688590041088ull, 289360691352043520ull, 4328521728ull, 196608ull, 513ull, 1028ull, 2064ull, },
                    { 15728640ull, 36099303470530560ull, 578721382704087040ull, 1108168671232ull, 458752ull, 1026ull, 2056ull, 4128ull, },
                    { 14680064ull, 141012903133184ull, 1157442765408174080ull, 283691314053120ull, 983040ull, 2052ull, 4112ull, 8256ull, },
                    { 12582912ull, 550829555712ull, 2314885530816348160ull, 72624976666034176ull, 2031616ull, 4104ull, 8224ull, 16512ull, },
                    { 8388608ull, 2147483648ull, 4629771061632696320ull, 145249953332068352ull, 4128768ull, 8208ull, 16448ull, 32768ull, },
                    { 0ull, 0ull, 9259542123265392640ull, 290499906664136704ull, 8323072ull, 16416ull, 32896ull, 0ull, },
                    { 4261412864ull, 1155177711056977920ull, 72340172821233664ull, 0ull, 0ull, 0ull, 65793ull, 132104ull, },
                    { 4227858432ull, 2310355422113955840ull, 144680345642467328ull, 4294967296ull, 16777216ull, 65536ull, 131586ull, 264208ull, },
                    { 4160749568ull, 4620710844227911680ull, 289360691284934656ull, 1108101562368ull, 50331648ull, 131328ull, 263172ull, 528416ull, },
                    { 4026531840ull, 9241421688455823360ull, 578721382569869312ull, 283691179835392ull, 117440512ull, 262657ull, 526344ull, 1056832ull, },
                    { 3758096384ull, 36099303202095104ull, 1157442765139738624ull, 72624976397598720ull, 251658240ull, 525314ull, 1052688ull, 2113664ull, },
                    { 3221225472ull, 141012366262272ull, 2314885530279477248ull, 145249952795197440ull, 520093696ull, 1050628ull, 2105376ull, 4227072ull, },
                    { 2147483648ull, 549755813888ull, 4629771060558954496ull, 290499905590394880ull, 1056964608ull, 2101256ull, 4210752ull, 8388608ull, },
                    { 0ull, 0ull, 9259542121117908992ull, 580999811180789760ull, 2130706432ull, 4202512ull, 8421504ull, 0ull, },
                    { 1090921693184ull, 577588851233521664ull, 72340168526266368ull, 0ull, 0ull, 0ull, 16843009ull, 33818640ull, },
                    { 1082331758592ull, 1155177702467043328ull, 144680337052532736ull, 1099511627776ull, 4294967296ull, 16777216ull, 33686018ull, 67637280ull, },
                    { 1065151889408ull, 2310355404934086656ull, 289360674105065472ull, 283673999966208ull, 12884901888ull, 33619968ull, 67372036ull, 135274560ull, },
                    { 1030792151040ull, 4620710809868173312ull, 578721348210130944ull, 72624942037860352ull, 30064771072ull, 67240192ull, 134744072ull, 270549120ull, },
                    { 962072674304ull, 9241421619736346624ull, 1157442696420261888ull, 145249884075720704ull, 64424509440ull, 134480385ull, 269488144ull, 541097984ull, },
                    { 824633720832ull, 36099165763141632ull, 2314885392840523776ull, 290499768151441408ull, 133143986176ull, 268960770ull, 538976288ull, 1082130432ull, },
                    { 549755813888ull, 140737488355328ull, 4629770785681047552ull, 580999536302882816ull, 270582939648ull, 537921540ull, 1077952576ull, 2147483648ull, },
                    { 0ull, 0ull, 9259541571362095104ull, 1161999072605765632ull, 545460846592ull, 1075843080ull, 2155905152ull, 0ull, },
                    { 279275953455104ull, 288793326105133056ull, 72339069014638592ull, 0ull, 0ull, 0ull, 4311810305ull, 8657571872ull, },
                    { 277076930199552ull, 577586652210266112ull, 144678138029277184ull, 281474976710656ull, 1099511627776ull, 4294967296ull, 8623620610ull, 17315143744ull, },
                    { 272678883688448ull, 1155173304420532224ull, 289356276058554368ull, 72620543991349248ull, 3298534883328ull, 8606711808ull, 17247241220ull, 34630287488ull, },
                    { 263882790666240ull, 2310346608841064448ull, 578712552117108736ull, 145241087982698496ull, 7696581394432ull, 17213489152ull, 34494482440ull, 69260574720ull, },
                    { 246290604621824ull, 4620693217682128896ull, 1157425104234217472ull, 290482175965396992ull, 16492674416640ull, 34426978560ull, 68988964880ull, 138521083904ull, },
                    { 211106232532992ull, 9241386435364257792ull, 2314850208468434944ull, 580964351930793984ull, 34084860461056ull, 68853957121ull, 137977929760ull, 277025390592ull, },
                    { 140737488355328ull, 36028797018963968ull, 4629700416936869888ull, 1161928703861587968ull, 69269232549888ull, 137707914242ull, 275955859520ull, 549755813888ull, },
                    { 0ull, 0ull, 9259400833873739776ull, 2323857407723175936ull, 139637976727552ull, 275415828484ull, 551911719040ull, 0ull, },
                    { 71494644084506624ull, 144115188075855872ull, 72057594037927936ull, 0ull, 0ull, 0ull, 1103823438081ull, 2216338399296ull, },
                    { 70931694131085312ull, 288230376151711744ull, 144115188075855872ull, 72057594037927936ull, 281474976710656ull, 1099511627776ull, 2207646876162ull, 4432676798592ull, },
                    { 69805794224242688ull, 576460752303423488ull, 288230376151711744ull, 144115188075855872ull, 844424930131968ull, 2203318222848ull, 4415293752324ull, 8865353596928ull, },
                    { 67553994410557440ull, 1152921504606846976ull, 576460752303423488ull, 288230376151711744ull, 1970324836974592ull, 4406653222912ull, 8830587504648ull, 17730707128320ull, },
                    { 63050394783186944ull, 2305843009213693952ull, 1152921504606846976ull, 576460752303423488ull, 4222124650659840ull, 8813306511360ull, 17661175009296ull, 35461397479424ull, },
                    { 54043195528445952ull, 4611686018427387904ull, 2305843009213693952ull, 1152921504606846976ull, 8725724278030336ull, 17626613022976ull, 35322350018592ull, 70918499991552ull, },
                    { 36028797018963968ull, 9223372036854775808ull, 4611686018427387904ull, 2305843009213693952ull, 17732923532771328ull, 35253226045953ull, 70644700037184ull, 140737488355328ull, },
                    { 0ull, 0ull, 9223372036854775808ull, 4611686018427387904ull, 35747322042253312ull, 70506452091906ull, 141289400074368ull, 0ull, },
                    { 18302628885633695744ull, 0ull, 0ull, 0ull, 0ull, 0ull, 282578800148737ull, 567382630219904ull, },
                    { 18158513697557839872ull, 0ull, 0ull, 0ull, 72057594037927936ull, 281474976710656ull, 565157600297474ull, 1134765260439552ull, },
                    { 17870283321406128128ull, 0ull, 0ull, 0ull, 216172782113783808ull, 564049465049088ull, 1130315200594948ull, 2269530520813568ull, },
                    { 17293822569102704640ull, 0ull, 0ull, 0ull, 504403158265495552ull, 1128103225065472ull, 2260630401189896ull, 4539061024849920ull, },
                    { 16140901064495857664ull, 0ull, 0ull, 0ull, 1080863910568919040ull, 2256206466908160ull, 4521260802379792ull, 9078117754732544ull, },
                    { 13835058055282163712ull, 0ull, 0ull, 0ull, 2233785415175766016ull, 4512412933881856ull, 9042521604759584ull, 18155135997837312ull, },
                    { 9223372036854775808ull, 0ull, 0ull, 0ull, 4539628424389459968ull, 9024825867763968ull, 18085043209519168ull, 36028797018963968ull, },
                    { 0ull, 0ull, 0ull, 0ull, 9151314442816847872ull, 18049651735527937ull, 36170086419038336ull, 0ull, },
            };
};


#endif //LCOS10GINE_BOARDRAYS_H
