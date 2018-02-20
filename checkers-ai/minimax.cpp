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

int MiniMaxHelper::totalNodes = 0;

CheckersGame::MovePackage ai::minimaxMove(CheckersGame & game, int depth, char maximizingPlayer) {
    int bestMoveVal = INT_MIN;
    MovePackage bestMove;

    auto moves = game.getValidMoves();

    for (auto i = 0; i < (int) moves.size(); ++i) {
        auto move = moves[i];
        auto moveVal = minimax(move, depth - 1, game.getActivePlayerColor(), game);

        if (moveVal > bestMoveVal) {
            bestMoveVal = moveVal;
            bestMove = move;
        }
    }

    cout << "Nodes evaluated: " << MiniMaxHelper::totalNodes << endl;
    return bestMove;
}


CheckersGame::JumpPackage ai::minimaxJump(CheckersGame & game, int depth, char maximizingPlayer) {
    int bestJumpVal = INT_MIN;
    JumpPackage bestJump;

    auto jumps = game.getValidJumps();

    for (auto i = 0; i < (int) jumps.size(); ++i) {
        auto jump = jumps[i] ;
        auto jumpVal = minimax(jump, depth - 1, game.getActivePlayerColor(), game);

        if (jumpVal > bestJumpVal) {
            bestJumpVal = jumpVal;
            bestJump = jump;
        }
    }

    cout << "Nodes evaluated: " << MiniMaxHelper::totalNodes << endl;

    return bestJump;

}


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

int MiniMaxHelper::recurse(const MovePackage & move, int depth) {
    totalNodes++;
    auto stateBeforeMove = getCurrentGameState();

    changeGameState(move);

    int bestNumPieces = isBaseCase(depth) ?
        baseCase() :
        recursiveCase(depth);

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

int MiniMaxHelper::recurse(const JumpPackage & jump, int depth) {
    totalNodes++;
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

    for (auto & jump : multiJumps) {
        auto jumpVal = recurse(jump, depth - 1);

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
        for (auto & jump : jumps) {
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
    return depth == 0 or !(game.areJumps() or game.areMoves());
}

int MiniMaxHelper::baseCase() {
    auto numPieces = game.getNumPiecesFor(maximizingPlayer);

    return numPieces;
}

void MiniMaxHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
    game.activePlayer = gameState.activePlayerColor == 'r' ? game.red : game.black;
    game.inactivePlayer = gameState.activePlayerColor == 'r' ? game.black : game.red;
}
