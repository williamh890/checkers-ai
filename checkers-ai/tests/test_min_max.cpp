#include "catch.hpp"
#include "../headers/min_max.h"
using ai::minimax;

#include "../headers/checkers-game.h"
using ai::getCheckersGame;
using ai::CheckersGame;

#include<utility>
using std::make_pair;

TEST_CASE("test minimax search funciton") {
    auto game = getCheckersGame();

    SECTION("test base case") {
        auto move = game.getValidMoves()[0];
        REQUIRE(minimax(move, 0, 'r', game) == 12);
        REQUIRE(minimax(move, 0, 'b', game) == 12);
    }

    SECTION("test depth 1") {
        auto move = game.getValidMoves()[0];
        REQUIRE(minimax(move, 1, 'r', game) == 12);
        REQUIRE(minimax(move, 1, 'b', game) == 12);
    }
}
