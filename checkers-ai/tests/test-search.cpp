#include "catch.hpp"

#include "../headers/search.h"
using ai::search;
using ai::SearchHelper;

#include "../headers/checkers-game.h"
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;
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


TEST_CASE("test search function", "[search]") {
    auto game = getCheckersGame();

    SECTION("test base case") {
        SECTION("all base cases from init board") {
            for ( auto & move : game.getValidMoves() ) {
                REQUIRE(search(move, 0, 'r', game) == 0);
                REQUIRE(search(move, 0, 'b', game) == 0);
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

            REQUIRE(search(make_pair(17, Jump(10, 14)), 10, 'b', game) == 3);
        }

        SECTION("search doesn't change the game state when run") {
            auto redPieces = game.red->getPieces();
            auto blackPieces = game.black->getPieces();
            auto boardState = game.board.getBoardState();

            for ( auto & move : game.getValidMoves() ) {
                search(move, 3, 'r', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);

            }

            for ( auto & move : game.getValidMoves() ) {
                search(move, 3, 'b', game);

                REQUIRE(game.red->getPieces() == redPieces);
                REQUIRE(game.black->getPieces() == blackPieces);
                REQUIRE(game.board.getBoardState() == boardState);
            }
        }
    }

    SECTION("test recursion depth of 1") {
        for ( auto & move : game.getValidMoves() ) {
            REQUIRE(search(move, 1, 'r', game) == 0);
            REQUIRE(search(move, 1, 'b', game) == 0);
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
            REQUIRE(search(jump, 1, 'r', game) == -1);
            REQUIRE(search(jump, 1, 'b', game) == 1);
        }
    }

}

TEST_CASE("search jumps recursion", "[search], [search-jumps]") {
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

        SECTION("jump was taken") {
            REQUIRE(search(setupJumpForRed, 1, 'b', game) == -1);
        }
    }

    SECTION("will recursively jump the same piece") {
        game.swapPlayers();
        game.board.setBoardState({
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   'r',   ' ',
                'r',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                'b',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' '
        });
        game.red->setPieces({
            Piece('r', 8),
            Piece('r', 6)
        });
        game.black->setPieces({
            Piece('b', 16)
        });

        auto doubleJumpSetupMove = make_pair(8, 13);

        SECTION("both pieces were jumped") {
            REQUIRE(search(doubleJumpSetupMove, 3, 'b', game) == 1);
        }
    }

    SECTION("jumping stops when piece is crowned") {
        game.swapPlayers();
        game.board.setBoardState({
                'r',   ' ',   ' ',   ' ',
             ' ',   ' ',   'r',   ' ',
                'b',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' ',
                ' ',   ' ',   ' ',   ' ',
             ' ',   ' ',   ' ',   ' '
        });
        game.red->setPieces({
            Piece('r', 0),
            Piece('r', 6)
        });
        game.black->setPieces({
            Piece('b', 8)
        });

        auto crowningSetupMove = make_pair(0, 5);

        SECTION("only one piece was jumped") {
            REQUIRE(search(crowningSetupMove, 3, 'b', game) == 0);
        }
    }
}

TEST_CASE("networked checkers game") {
    int networkRedId=0, networkBlackId=1;
    auto game = getNetworkedCheckersGame(networkRedId, networkBlackId);

    game.board.setBoardState({
            'r',   ' ',   ' ',   ' ',
         ' ',   ' ',   'r',   ' ',
            'b',   ' ',   ' ',   ' ',
         ' ',   ' ',   ' ',   ' ',
            ' ',   ' ',   ' ',   ' ',
         ' ',   ' ',   ' ',   ' ',
            ' ',   ' ',   ' ',   ' ',
         ' ',   ' ',   ' ',   ' '
    });
    game.red->setPieces({
        Piece('r', 0),
        Piece('r', 6)
    });
    game.black->setPieces({
        Piece('b', 8)
    });

    game.swapPlayers();
    auto crowningSetupMove = make_pair(0, 5);

    REQUIRE_NOTHROW(search(crowningSetupMove, 3, 'b', game));
    auto helper = SearchHelper('b', game);
    REQUIRE_NOTHROW(game.activePlayer->baseCase(helper));
}

TEST_CASE ("minimax wrapper functions behave") {
    auto game = getCheckersGame();

    SECTION ("throws exception if depth == 0") {
        REQUIRE_THROWS(getBestMove(game, 0));
        REQUIRE_THROWS(getBestJump(game, 0, -1));
    }
}

void tableDivider () {
    cout << "|-------|---------------|-----------------------"
         << "|---------------|---------------|" << endl;
}

void runSearchHeader () {
    tableDivider();
    cout << "|Depth\t|Nodes evaluated|Nodes with pruning\t|Leaf Nodes\t|Total time (s)\t|" << endl;
    tableDivider();
}

void outputWithTabs(int numOut) {
    cout << numOut;
    if (numOut < 999999)
        cout << "\t";
}

vector<MovePackage> moves;
void runSearch(CheckersGame & game, int depth) {
    const int ITERATIONS = 1;

    SearchHelper::totalNodes = 0;
    SearchHelper::leafNodes = 0;
    SearchHelper::prunedNodes = 0;
    CheckersGame::SEARCH_DEPTH = depth;
    auto start = getTime();
    moves.push_back(game.getBestMove());
    auto end = getTime();

    auto total = (end - start);
    cout << "|" << depth << "\t|";
    // Nodes Evaluated
    outputWithTabs(SearchHelper::totalNodes);
    // Nodes with pruning
    cout << "\t|" << SearchHelper::prunedNodes << "\t\t\t|";
    // Leaf nodes
    outputWithTabs(SearchHelper::leafNodes);
    // Total time
    cout << "\t|" << total <<  "\t|" << endl;
    tableDivider();
}

TEST_CASE ("timing search at different depths", "[search],[timing]") {
    cout << endl << "search timing: \n" << endl;

    auto game = getCheckersGame();

    runSearchHeader();
    for (size_t d = 2; d <= 10; ++d) {
        runSearch(game, d);
    }

}
