#include "../headers/move-generator.h"
using ai::getGeneratorFor;
using ai::getKingGenerator;
using ai::MoveGenerator;
#include "../headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;
#include "../headers/models.h"
using ai::Jump;
#include "../headers/consts.h"
using ai::TOTAL_NUM_PIECES;

#include "catch.hpp"

TEST_CASE("testing move generator class") {
    auto json = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter(json);

    auto rJumps = converter.getJumpsFor("red");
    auto rMoves = converter.getMovesFor("red");

    auto bJumps = converter.getJumpsFor("black");
    auto bMoves = converter.getMovesFor("black");

    SECTION("Testing constructor for move generator") {
        auto redGenerator = MoveGenerator(rMoves, rJumps);
    }

    auto blackGenerator = MoveGenerator(bMoves, bJumps);
    auto redGenerator = MoveGenerator(rMoves, rJumps);

    SECTION("Testing if moves are correctly loaded") {
        for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
            auto moves = blackGenerator.getMoves(space);

            REQUIRE((moves.size() < 3 and moves.size() >= 0));
        }
    }

    SECTION("Test if jumps are correctly loaded") {
        for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
            auto jumps = blackGenerator.getJumps(space);

            REQUIRE((jumps.size() < 3 and jumps.size() >= 0));
        }
    }

    SECTION("Test if moves at space 3 are in correct format (\"3\": [])") {
        auto space = 3;
        auto moves = blackGenerator.getMoves(space);
        REQUIRE(moves.size() == 0);
    }

    SECTION("Test if moves at space 4 are correct (\"4\": [0])") {
        auto space = 4;
        auto moves = blackGenerator.getMoves(space);

        //   ,
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0] == 0);

    }

    SECTION("Test if moves at space 5 are correct (\"5\": [0)") {
        auto space = 5;
        auto moves = blackGenerator.getMoves(space);

        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0] == 0);
        REQUIRE(moves[1] == 1);
    }

    //"22": [[29, 25], [31, 26]],
    //\"23\": [[30, 26]],
    //,
    SECTION("Test if jumps at space 24 are in correct format \"24: []\"") {
        auto space = 24;
        auto jumps = redGenerator.getJumps(space);

        REQUIRE(jumps.size() == 0);
    }

    SECTION("Test if jumps at space 23 are in correct format \"23: [[30, 26]]\"") {
        auto space = 23;
        auto jumps = redGenerator.getJumps(space);

        REQUIRE(jumps.size() == 1);
        REQUIRE(jumps[0] == Jump(30, 26));
    }

    SECTION("Test if jumps at space 22 are in correct format \"22: [[29, 25], [31, 26]]\"") {
        auto space = 22;
        auto jumps = redGenerator.getJumps(space);

        REQUIRE(jumps.size() == 2);
        REQUIRE(jumps[0] == Jump(29, 25));
        REQUIRE(jumps[1] == Jump(31, 26));
    }

    SECTION("Test getKingGenerator factory function") {
        auto kingGen = getKingGenerator(converter);

        REQUIRE(kingGen.getJumps(22).size() == 4);
        REQUIRE(kingGen.getMoves(7).size() == 4);
    }

    SECTION("Test getGeneratorFor factory function") {
        auto factoryBlackGenerator = getGeneratorFor("black", converter);

        REQUIRE((factoryBlackGenerator.getJumps(28)[0] == blackGenerator.getJumps(28)[0]));
        REQUIRE((factoryBlackGenerator.getMoves(10)[1] == blackGenerator.getMoves(10)[1]));
    }
}
