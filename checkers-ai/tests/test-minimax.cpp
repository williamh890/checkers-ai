#include "catch.hpp"

#include "../headers/minimax.h"
using ai::minimax;

#include "../headers/checkers-game.h"
using ai::getCheckersGame;
using ai::CheckersGame;

#include "../headers/models.h"
using ai::Piece;

#include<utility>
using std::make_pair;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

TEST_CASE("test minimax search function", "[minimax]") {
    auto game = getCheckersGame();

    SECTION("test base case") {
        SECTION("testing all base cases from init board") {
            cout << "testing base case" << endl;
            for ( auto & move : game.getValidMoves() ) {
                REQUIRE(minimax(move, 0, 'r', game) == 12);
                REQUIRE(minimax(move, 0, 'b', game) == 12);
            }
        }

        SECTION("testing minimax doesn't change the game state when run") {
            auto redPieces = game.red->getPieces();
            auto blackPieces = game.black->getPieces();
            auto boardState = game.board.getBoardState();

            for ( auto & move : game.getValidMoves() ) {
                minimax(move, 0, 'r', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);

            }

            for ( auto & move : game.getValidMoves() ) {
                minimax(move, 0, 'b', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);
            }
        }
    }

    SECTION("test recursion depth of 1") {
        for ( auto & move : game.getValidMoves() ) {
            REQUIRE(minimax(move, 1, 'r', game) == 12);
            REQUIRE(minimax(move, 1, 'b', game) == 12);
        }
    }

    SECTION("test that piece count changes if jump is inevitable.") {
        vector<char> sampleBoard ;

        game.board.setBoardState({
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                'r',   ' ',   ' ',   ' ',
             ' ',   ' ',   'r',   ' ',
                ' ',   'b',   'b',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' '
        });
        auto red = vector<Piece>{
                Piece('r', 8),
                Piece('r', 14)
        };
        auto black = vector<Piece>{
            Piece('b', 17),
            Piece('b', 18)
        };

        game.red->setPieces(red);
        game.black->setPieces(black);

        REQUIRE(game.getValidJumps().size() > 0);
        for (auto & jump : game.getValidJumps() ) {
            REQUIRE(minimax(jump, 1, 'r', game) == 1);
            REQUIRE(minimax(jump, 1, 'b', game) == 2);
        }
    }
}
