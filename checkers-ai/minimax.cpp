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

CheckersGame::MovePackage ai::minimaxMove(CheckersGame & game, int depth, char maximizingPlayer) {
    int bestMoveVal = INT_MIN;
    MovePackage bestMove;

    auto moves = game.getValidMoves();

    for (auto i = 0; i < (int) moves.size(); ++i) {
        auto move = moves[i];
        auto moveVal = minimax(move, depth, game.getActivePlayerColor(), game);

        if (moveVal > bestMoveVal) {
            bestMoveVal = moveVal;
            bestMove = move;
        }
    }

    return bestMove;
}

CheckersGame::JumpPackage ai::minimaxJump(CheckersGame & game, int depth, char maximizingPlayer) {
    int bestJumpVal = INT_MIN;
    JumpPackage bestJump;

    auto jumps = game.getValidJumps();

    for (auto i = 0; i < (int) jumps.size(); ++i) {
        auto jump = jumps[i] ;
        auto jumpVal = minimax(jump, depth, game.getActivePlayerColor(), game);

        if (jumpVal > bestJumpVal) {
            bestJumpVal = jumpVal;
            bestJump = jump;
        }
    }

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

int MiniMaxHelper::recurse(MovePackage move, int depth) {
    auto stateBeforeMove = getCurrentGameState();

    changeGameState(move);
    //cout << "AT DEPTH " << depth << endl;
    //cout << game.board.toString() << endl;

    int bestNumPieces = isBaseCase(depth) ?
        baseCase() :
        recursiveCase(depth);

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

int MiniMaxHelper::recurse(JumpPackage jump, int depth) {
    auto stateBeforeMove = getCurrentGameState();

    auto jumpDestination = changeGameState(jump);
    //cout << "AT DEPTH " << depth << endl;
    //cout << game.board.toString() << endl;

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
        auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
        bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;
        for (auto & jump : multiJumps) {
            auto jumpVal = recurse(jump, depth - 1);

            bestNumPieces = (isMaximizingPlayer) ?
                max(jumpVal, bestNumPieces) :
                min(jumpVal, bestNumPieces);
        }
    }
    else {
        game.swapPlayers();
        bestNumPieces = recursiveCase(depth);
    }


    setGameState(stateBeforeMove);
    return bestNumPieces;
}

int MiniMaxHelper::recursiveCase(int depth) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    int bestNumPieces = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

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


