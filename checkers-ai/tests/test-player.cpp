#include "../headers/player.h"
using ai::getPlayer;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::PlayerType;

#include "../headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "../headers/consts.h"
using ai::TOTAL_NUM_SPACES;
using ai::INIT_NUM_PIECES;

#include <memory>
using std::make_shared;

#include "catch.hpp"

TEST_CASE("testing player object"){
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};
    auto redGenerator = getGeneratorFor("red", converter);
    auto blackGenerator = getGeneratorFor("red", converter);
    auto kingGenerator = getKingGenerator(converter);

    SECTION("player constructor") {
        auto red = RedPlayer('r', redGenerator, kingGenerator, PlayerType::Computer);
    }

    auto red = make_shared<RedPlayer>('r', redGenerator, kingGenerator, PlayerType::Computer);
    auto black = make_shared<BlackPlayer>('b', blackGenerator, kingGenerator, PlayerType::Computer);

    SECTION("testing getColor") {
        REQUIRE(red->getColor() == 'r');
        REQUIRE(black->getColor() == 'b');
    }

    SECTION("testing getPieces") {
        REQUIRE((red->getPieces()).size() == 12);
        REQUIRE((black->getPieces()).size() == 12);
    }

    SECTION("testing if pieces are in correct spaces for red") {
        for (auto piece: red->getPieces()) {
            REQUIRE(piece.space < TOTAL_NUM_SPACES);
        }
    }

    SECTION("testing if pieces are in correct spaces for red") {
        for (auto piece: black->getPieces()) {
            REQUIRE(piece.space >= TOTAL_NUM_SPACES - INIT_NUM_PIECES);
        }
    }
}
