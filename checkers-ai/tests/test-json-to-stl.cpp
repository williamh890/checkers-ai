#include "../headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "catch.hpp"

TEST_CASE("testing loading move table from json and the json to stl converting process."){
    auto json = loadMoveTableFrom("tests/test.json");

    SECTION("testing loadMoveTableFrom(filename)") {
        REQUIRE(json["black"]["moves"]["29"][0] == 24);
        REQUIRE(json["black"]["jumps"]["1"][0][0] == 8);
    }

    SECTION("testing JsonToStlConverter constructor") {
        auto converter = JsonToStlConverter{json};
    }

    auto converter = JsonToStlConverter{json};
    SECTION("testing getting moves") {
        auto moves = converter.getMovesFor("red");

        REQUIRE(moves[5].size() == 2);
        REQUIRE(moves[5][0] == 0);
    }

    SECTION("testing getting jumps") {
        auto jumps = converter.getJumpsFor("black");

        REQUIRE(jumps[1].size() == 2);
        REQUIRE(jumps[1][0].to == 8);
        REQUIRE(jumps[1][0].through == 5);
    }
}
