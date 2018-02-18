#include "headers/minimax.h"
using ai::minimax;
using ai::MiniMaxHelper;
using ai::GameState;

#include "headers/models.h"
using ai::Piece;
using ai::Jump;

#include "headers/checkers-game.h"
using ai::CheckersGame;
using JumpPackage = CheckersGame::JumpPackage;
using MovePackage = CheckersGame::MovePackage;

#include <climits>
#include <algorithm>
using std::max;
using std::min;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

using Pieces = vector<Piece>;
using BoardState = vector<char>;

int ai::minimax(MovePackage move, int depth, char maximizingPlayer, CheckersGame & game) {
    MiniMaxHelper minimax(maximizingPlayer, game);

    return minimax.recurse(move, depth);
}

int ai::minimax(JumpPackage jump, int depth, char maximizingPlayer, CheckersGame & game) {
    MiniMaxHelper minimax(maximizingPlayer, game);

    return minimax.recurse(jump, depth);
}

MiniMaxHelper::MiniMaxHelper(char maximizingPlayer, CheckersGame & game) :
    game(game),
    maximizingPlayer(maximizingPlayer) {
    }

GameState::GameState(const BoardState & board, const Pieces & red, const Pieces & black, char activePlayerColor):
    boardState(board),
    redPieces(red),
    blackPieces(black),
    activePlayerColor(activePlayerColor) {
    }

int MiniMaxHelper::recurse(MovePackage move, int depth) {
    auto stateBeforeMove = getCurrentGameState();

    applyAction(move);

    int bestNumPieces;
    if (isBaseCase(depth)) {
        bestNumPieces = handleBaseCase();
    }
    // recursive case
    else {
        auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;

        bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

        for (auto & checkMove : game.getValidMoves()) {
            auto moveValue = recurse(checkMove, depth - 1);

            bestNumPieces = (isMaximizingPlayer) ?
                max(moveValue, bestNumPieces ) :
                min(moveValue, bestNumPieces);
        }
    }

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

int MiniMaxHelper::recurse(JumpPackage jump, int depth) {
    auto stateBeforeMove = getCurrentGameState();

    auto jumpDestination = applyAction(jump);

    if (isBaseCase(depth)) {
        setGameState(stateBeforeMove);
        return handleBaseCase();
    }

    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    int bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

    // Either recurse on moves or jumps
    if (game.getValidJumps().size() != 0) {
        for (auto & jump : game.getValidJumps()) {
            auto jumpVal = recurse(jump, depth - 1);

            bestNumPieces = (isMaximizingPlayer) ?
                max(jumpVal, bestNumPieces) :
                min(jumpVal, bestNumPieces);
        }
    }
    else {
        for (auto & move : game.getValidMoves()) {
            auto moveVal = recurse(move, depth - 1);

            bestNumPieces = (isMaximizingPlayer) ?
                max(moveVal, bestNumPieces) :
                min(moveVal, bestNumPieces);
        }
    }

    setGameState(stateBeforeMove);
    return bestNumPieces;
}


GameState MiniMaxHelper::getCurrentGameState() {
    return GameState(
            game.board.getBoardState(),
            game.red->getPieces(),
            game.black->getPieces(),
            game.activePlayer->getColor()
            );
}

void MiniMaxHelper::applyAction(const MovePackage & move) {
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();
}

int MiniMaxHelper::applyAction(const JumpPackage & jump) {
    game.board.make(jump);
    game.reactTo(jump);

    return jump.second.to;
}


bool MiniMaxHelper::isBaseCase(int depth) {
    return depth == 0;
}

int MiniMaxHelper::handleBaseCase() {
    return game.getNumPiecesFor(maximizingPlayer);
}

void MiniMaxHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
    game.activePlayer = gameState.activePlayerColor == 'r' ? game.red : game.black;
}


