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
    MiniMaxHelper mm(depth, maximizingPlayer, game);

    return mm.recurse(move);
}

MiniMaxHelper::MiniMaxHelper(int depth, char maximizingPlayer, CheckersGame & game) :
    game(game),
    maximizingPlayer(maximizingPlayer),
    depth(depth) {
    }

GameState::GameState(const BoardState & board, const Pieces & red, const Pieces & black):
    boardState(board),
    redPieces(red),
    blackPieces(black) {
    }

int MiniMaxHelper::recurse(MovePackage move) {
    auto stateBeforeMove = getCurrentGameState();

    applyMoveTo(move);

    int best;
    if (isBaseCase()) {
        best = handleBaseCase();
    }
    else {
        depth -= 1;
        best = recursiveCase();
    }

    setGameState(stateBeforeMove);

    depth += 1;
    return best;
}

GameState MiniMaxHelper::getCurrentGameState() {
    return GameState(
            game.board.getBoardState(),
            game.red->getPieces(),
            game.black->getPieces()
            );
}

void MiniMaxHelper::applyMoveTo(const MovePackage & move) {
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();
}

bool MiniMaxHelper::isBaseCase() {
    return depth == 0;
}

int MiniMaxHelper::handleBaseCase() {
    return game.getNumPiecesFor(maximizingPlayer);
}

int MiniMaxHelper::recursiveCase() {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;

    int best = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

    for (auto & checkMove : game.getValidMoves()) {
        auto moveValue = recurse(checkMove);

        best = (isMaximizingPlayer) ?
            max(moveValue, best) :
            min(moveValue, best);
    }

    return best;
}

void MiniMaxHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
}


