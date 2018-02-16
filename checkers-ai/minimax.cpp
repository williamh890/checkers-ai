#include "headers/min_max.h"
#include "headers/models.h"
using ai::Piece;
using ai::Jump;
#include "headers/checkers-game.h"
using ai::CheckersGame;
#include <climits>
#include <algorithm>
using std::max;
using std::min;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

struct GameState {
    vector<char> boardState;
    vector<Piece> redPieces;
    vector<Piece> blackPieces;
};

void applyMoveTo(CheckersGame & game, const std::pair<int, int> & move) {
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();
}

void applyJumpTo(CheckersGame & game, const std::pair<int, Jump> & jump) {
    game.board.make(jump);
    game.reactTo(jump);
    game.swapPlayers();
}

GameState getStateFrom(CheckersGame & game) {

    // TODO!! SAVE BOARD STATE AND PLAYER PIECES HERE
    return GameState();
}

void setStateIn(CheckersGame & game, GameState & gameState) {
    // TODO!! REVERT BOARD STATE AND PLAYER PIECES CHANGES HERE
}


int ai::minimax(std::pair<int, int> move, int depth, char maximizingPlayer, CheckersGame & game) {
    auto stateBeforeMove = getStateFrom(game);

    applyMoveTo(game, move);
    int best;
    // base case
    if (depth == 0) {
        best = game.getNumPiecesFor(maximizingPlayer);
    }
    // recursive case
    else {
        if (game.activePlayer->getColor() == maximizingPlayer) {
            best = INT_MIN;

            // TODO!! GET JUMPS OR MOVES AND CALL MINIMAX RESPECTIVELY
            for (auto currMoves : game.getValidMoves()) {
                auto moveValue = minimax(currMoves, depth - 1, maximizingPlayer, game);
                best = max(moveValue, best);
            }

        } else {
            best = INT_MAX;

            for (auto currMoves : game.getValidMoves()) {
                auto moveValue = minimax(currMoves, depth - 1, maximizingPlayer, game);
                best = min(moveValue, best);
            }
        }
    }

    setStateIn(game, stateBeforeMove);

    return best;
}

int minimax(std::pair<int, Jump> jump, int depth, char maximizingPlayer, CheckersGame & game) {
    auto stateBeforeMove = getStateFrom(game);

    applyJumpTo(game, jump);

    // TODO!! HANDLE MINIMAX FOR JUMPS, SHOULD BE VERY SIMILAR.
}

