#include "catch.hpp"
#include "../headers/minimax.h"
using ai::minimax;

#include "../headers/checkers-game.h"
using ai::getCheckersGame;
using ai::CheckersGame;

#include<utility>
using std::make_pair;
#include <iostream>
using std::cout;
using std::endl;

TEST_CASE("test minimax search funciton") {
    auto game = getCheckersGame();

    SECTION("test base case") {
        cout << "testing base case" << endl;
        for ( auto & move : game.getValidMoves() ) {
            REQUIRE(minimax(move, 0, 'r', game) == 12);
            REQUIRE(minimax(move, 0, 'b', game) == 12);
        }
    }
}
