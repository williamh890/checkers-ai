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
    maximizingPlayer(maximizingPlayer)
    {
    }

GameState::GameState(const BoardState & board, const Pieces & red, const Pieces & black):
    boardState(board),
    redPieces(red),
    blackPieces(black) {
    }

int MiniMaxHelper::recurse(MovePackage move, int depth) {
    auto stateBeforeMove = getCurrentGameState();

    applyMoveTo(move);

    int best;
    if (isBaseCase(depth)) {
        best = handleBaseCase();
    }
    else {
        best = recursiveCase(depth);
    }

    setGameState(stateBeforeMove);

    return best;
}

int MiniMaxHelper::recurse(JumpPackage jump, int depth) {
    return 0;
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

bool MiniMaxHelper::isBaseCase(int depth) {
    return depth == 0;
}

int MiniMaxHelper::handleBaseCase() {
    return game.getNumPiecesFor(maximizingPlayer);
}

int MiniMaxHelper::recursiveCase(int depth) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;

    int best = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

    for (auto & checkMove : game.getValidMoves()) {
        auto moveValue = recurse(checkMove, depth - 1);

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


