#include "catch.hpp"

#include "../headers/minimax.h"
using ai::minimax;
using ai::MiniMaxHelper;

#include "../headers/checkers-game.h"
using ai::getCheckersGame;
using ai::CheckersGame;
using MovePackage = CheckersGame::MovePackage;

#include "../headers/models.h"
using ai::Piece;
using ai::Jump;

#include "../headers/utils.h"
using ai::getTime;

#include<utility>
using std::make_pair;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <chrono>


TEST_CASE("test minimax search function", "[minimax]") {
    auto game = getCheckersGame();

    SECTION("test base case") {
        SECTION("all base cases from init board") {
            for ( auto & move : game.getValidMoves() ) {
                REQUIRE(minimax(move, 0, 'r', game) == 0);
                REQUIRE(minimax(move, 0, 'b', game) == 0);
            }
        }

        SECTION("base case when a player is out of moves") {
            game.board.setBoardState({
                    ' ',   ' ',   ' ',   ' ',
                 ' ',   ' ',   ' ',   ' ',
                    ' ',   ' ',   ' ',   ' ',
                 ' ',   ' ',   'r',   ' ',
                    ' ',   'b',   'b',   ' ',
                 ' ',   ' ',   ' ',   ' ',
                    ' ',   ' ',   ' ',   ' ',
                 ' ',   ' ',   'b',   ' '
            });
            game.red->setPieces({
                Piece('r', 14)
            });
            game.black->setPieces({
                Piece('b', 17),
                Piece('b', 18),
                Piece('b', 30)
            });

            REQUIRE(minimax(make_pair(17, Jump(10, 14)), 10, 'b', game) == 3);
        }

        SECTION("minimax doesn't change the game state when run") {
            auto redPieces = game.red->getPieces();
            auto blackPieces = game.black->getPieces();
            auto boardState = game.board.getBoardState();

            for ( auto & move : game.getValidMoves() ) {
                minimax(move, 3, 'r', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);

            }

            for ( auto & move : game.getValidMoves() ) {
                minimax(move, 3, 'b', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);
            }
        }
    }

    SECTION("test recursion depth of 1") {
        for ( auto & move : game.getValidMoves() ) {
            REQUIRE(minimax(move, 1, 'r', game) == 0);
            REQUIRE(minimax(move, 1, 'b', game) == 0);
        }
    }

    SECTION("piece count changes if jump is inevitable.") {
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
        game.red->setPieces({
            Piece('r', 8),
            Piece('r', 14)
        });
        game.black->setPieces({
            Piece('b', 17),
            Piece('b', 18)
        });

        REQUIRE(game.getValidJumps().size() > 0);
        for (auto & jump : game.getValidJumps() ) {
            REQUIRE(minimax(jump, 1, 'r', game) == -1);
            REQUIRE(minimax(jump, 1, 'b', game) == 1);
        }
    }

}

TEST_CASE("minimax jumps recursion", "[minimax], [minimax-jumps]") {
    auto game = getCheckersGame();

    SECTION("handle jumps before moves in move recurse") {
        game.board.setBoardState({
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   'r',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                'b',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' '
        });
        game.red->setPieces({
            Piece('r', 17)
        });
        game.black->setPieces({
            Piece('b', 24)
        });

        auto setupJumpForRed = make_pair(24, 21);

        REQUIRE(minimax(setupJumpForRed, 1, 'b', game) == -1);
    }

    SECTION("will recursively jump the same piece") {
        game.board.setBoardState({
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   'r',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   'b',   ' ',
                'b',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' '
        });
        game.red->setPieces({
            Piece('r', 14)
        });
        game.black->setPieces({
            Piece('b', 24),
            Piece('b', 22)
        });

        auto doubleJumpSetupMove = make_pair(22, 17);

        REQUIRE(minimax(doubleJumpSetupMove, 3, 'b', game) == -1);
    }
}

TEST_CASE ("minimax wrapper functions behave") {
    auto game = getCheckersGame();

    SECTION ("throws exception if depth == 0") {
        REQUIRE_THROWS(minimaxMove(game, 0, 'b'));
        REQUIRE_THROWS(minimaxJump(game, 0, 'b'));
    }
}

vector<MovePackage> moves;
TEST_CASE ("timing minimax at different depths", "[minimax],[timing]") {
    auto game = getCheckersGame();

    const int ITERATIONS = 1;

    vector<int> dummy;
    auto start = getTime();
    for (volatile int i = 0; i < ITERATIONS; ++i) {
        dummy.push_back((int)i);
    }
    auto end = getTime();
    auto pushBackTotal = end - start;

    MiniMaxHelper::totalNodes = 0;
    start = getTime();
    for (volatile int i = 0; i < ITERATIONS; ++i) {
        moves.push_back(game.getMinimaxMove());
    }
    end = getTime();

    auto total = (end - start) - pushBackTotal;

    cout << "Total time per search: " << total / ITERATIONS << endl;
}

