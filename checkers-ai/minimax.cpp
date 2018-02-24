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
#include <exception>
using std::invalid_argument;

using Pieces = vector<Piece>;
using BoardState = vector<char>;

int MiniMaxHelper::totalNodes = 0;

<<<<<<< HEAD
CheckersGame::MovePackage ai::minimaxMove(CheckersGame & game, int depth, char maximizingPlayer) {
    int bestMoveVal = INT_MIN, moveVal;
    MovePackage bestMove, move;
=======
CheckersGame::MovePackage ai::minimaxMove(CheckersGame & game, int depth) {
    int bestMoveVal = INT_MIN;
    MovePackage bestMove;
>>>>>>> 74493fe7accbade0dba451b8fda30c09f5dfffa9

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto moves = game.getValidMoves();
    MovePackage move;
    int moveVal;

    for (auto i = 0; i < (int) moves.size(); ++i) {
        move = moves[i];
        moveVal = minimax(move, depth - 1, game.getActivePlayerColor(), game);

        if (moveVal > bestMoveVal) {
            bestMoveVal = moveVal;
            bestMove = move;
        }
    }

    return bestMove;
}


CheckersGame::JumpPackage ai::minimaxJump(CheckersGame & game, int depth) {
    int bestJumpVal = INT_MIN;
    JumpPackage bestJump;

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto jumps = game.getValidJumps();
    JumpPackage jump;
    int jumpVal;

    for (auto i = 0; i < (int) jumps.size(); ++i) {
        jump = jumps[i] ;
        jumpVal = minimax(jump, depth - 1, game.getActivePlayerColor(), game);

        if (jumpVal > bestJumpVal) {
            bestJumpVal = jumpVal;
            bestJump = jump;
        }
    }

    return bestJump;
}


int ai::minimax(const MovePackage & move, int depth, char maximizingPlayer, CheckersGame & game) {
    MiniMaxHelper minimax(maximizingPlayer, game);

    return minimax.recurse(move, depth);
}

int ai::minimax(const JumpPackage & jump, int depth, char maximizingPlayer, CheckersGame & game) {
    MiniMaxHelper minimax(maximizingPlayer, game);

    return minimax.recurse(jump, depth);
}

MiniMaxHelper::MiniMaxHelper(char maximizingPlayer, CheckersGame & game) :
    game(game),
    maximizingPlayer(maximizingPlayer) {
    }

GameState::GameState(
        const BoardState & board,
        const Pieces & red,
        const Pieces & black,
        char activePlayerColor):
    boardState(board),
    redPieces(red),
    blackPieces(black),
    activePlayerColor(activePlayerColor) {
    }

int MiniMaxHelper::recurse(const MovePackage & move, int depth) {
    ++totalNodes;
    auto stateBeforeMove = getCurrentGameState();

    changeGameState(move);

    int bestNumPieces = isBaseCase(depth) ?
        baseCase() :
        recursiveCase(depth);

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

int MiniMaxHelper::recurse(const JumpPackage & jump, int depth) {
    ++totalNodes;
    auto stateBeforeMove = getCurrentGameState();

    auto jumpDestination = changeGameState(jump);

    game.swapPlayers();
    if (isBaseCase(depth)) {
        int numPieces = baseCase();
        setGameState(stateBeforeMove);

        return numPieces;
    }
    game.swapPlayers();

    auto multiJumps = game.getValidJumpsAt(jumpDestination);
    int bestNumPieces;

    if (multiJumps.size() > 0) {
        bestNumPieces = recurseMultiJumpCase(multiJumps, depth);
    }
    else {
        game.swapPlayers();
        bestNumPieces = recursiveCase(depth);
    }

    setGameState(stateBeforeMove);
    return bestNumPieces;
}

int MiniMaxHelper::recurseMultiJumpCase(const vector<JumpPackage> & multiJumps, int depth) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    int bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;
    int jumpVal;

    for (auto & jump : multiJumps) {
        jumpVal = recurse(jump, depth);

        bestNumPieces = (isMaximizingPlayer) ?
            max(jumpVal, bestNumPieces) :
            min(jumpVal, bestNumPieces);
    }

    return bestNumPieces;
}

int MiniMaxHelper::recursiveCase(int depth) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    int bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

    auto jumps = game.getValidJumps();

    if (jumps.size() != 0) {
        int jumpVal;
        for (auto & jump : jumps) {
            jumpVal = recurse(jump, depth);

            bestNumPieces = (isMaximizingPlayer) ?
                max(jumpVal, bestNumPieces) :
                min(jumpVal, bestNumPieces);
        }
    }
    else {
        int moveVal;
        for (auto & move : game.getValidMoves()) {
            moveVal = recurse(move, depth - 1);

            bestNumPieces = (isMaximizingPlayer) ?
                max(moveVal, bestNumPieces) :
                min(moveVal, bestNumPieces);
        }
    }

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

void MiniMaxHelper::changeGameState(const MovePackage & move) {
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();
}

int MiniMaxHelper::changeGameState(const JumpPackage & jump) {
    game.board.make(jump);
    game.reactTo(jump);

    return jump.second.to;
}

bool MiniMaxHelper::isBaseCase(int depth) {
    ++totalNodes;
    return depth == 0 or !(game.areJumps() or game.areMoves());
}

int MiniMaxHelper::baseCase() {
    auto numPieces = game.getNumPiecesFor(maximizingPlayer);

    char opponentColor = (maximizingPlayer == 'r') ? 'b' : 'r';
    auto numEnemyPieces = game.getNumPiecesFor(opponentColor);

    return numPieces - numEnemyPieces;
}

void MiniMaxHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
    game.activePlayer = gameState.activePlayerColor == 'r' ? game.red : game.black;
    game.inactivePlayer = gameState.activePlayerColor == 'r' ? game.black : game.red;
}
