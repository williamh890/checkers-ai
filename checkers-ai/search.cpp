#include "headers/search.h"
using ai::EvaluationType;
using ai::search;
using ai::SearchHelper;
using ai::GameState;

#include "headers/models.h"
using ai::Piece;
using ai::Jump;
using ai::PostJumpInformation;

#include "headers/checkers-game.h"
using ai::CheckersGame;
using JumpPackage = CheckersGame::JumpPackage;
using MovePackage = CheckersGame::MovePackage;

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

int SearchHelper::totalNodes = 0;
int SearchHelper::prunedNodes = 0;

CheckersGame::MovePackage ai::getBestMove(CheckersGame & game, int depth) {
    EvaluationType bestMoveVal = -INFINITY;
    MovePackage bestMove;

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto moves = game.getValidMoves();
    MovePackage move;
    EvaluationType moveVal;

    for (auto i = 0; i < (int) moves.size(); ++i) {
        move = moves[i];
        moveVal = search(move, depth - 1, game.getActivePlayerColor(), game);

        if (moveVal > bestMoveVal) {
            bestMoveVal = moveVal;
            bestMove = move;
        }
    }

    return bestMove;
}

CheckersGame::JumpPackage ai::getBestJump(CheckersGame & game, int depth, int space) {
    EvaluationType bestJumpVal = -INFINITY;

    JumpPackage bestJump;

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto jumps = space == -1 ? game.getValidJumps() : game.getValidJumpsAt(space);
    JumpPackage jump;
    EvaluationType jumpVal;

    for (auto i = 0; i < (int) jumps.size(); ++i) {
        jump = jumps[i] ;
        jumpVal = search(jump, depth - 1, game.getActivePlayerColor(), game);

        if (jumpVal > bestJumpVal) {
            bestJumpVal = jumpVal;
            bestJump = jump;
        }
    }

    return bestJump;
}


EvaluationType ai::search(
        const MovePackage & move,
        int depth,
        char maximizingPlayer,
        CheckersGame & game) {
    SearchHelper search(maximizingPlayer, game);

    EvaluationType alpha=-INFINITY, beta=INFINITY;
    return search.recurse(move, depth, alpha, beta);
}

EvaluationType ai::search(
        const JumpPackage & jump,
        int depth,
        char maximizingPlayer,
        CheckersGame & game) {

    SearchHelper search(maximizingPlayer, game);

    EvaluationType alpha=-INFINITY, beta=INFINITY;
    return search.recurse(jump, depth, alpha, beta);
}

SearchHelper::SearchHelper(char maximizingPlayer, CheckersGame & game) :
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

EvaluationType SearchHelper::recurse(
        const MovePackage & move,
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    ++totalNodes;
    auto stateBeforeMove = getCurrentGameState();

    changeGameState(move);

    EvaluationType bestNumPieces = isBaseCase(depth) ?
        game.activePlayer->baseCase(*this) :
        recursiveCase(depth, alpha, beta);

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

EvaluationType SearchHelper::recurse(
        const JumpPackage & jump,
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    ++totalNodes;
    auto stateBeforeMove = getCurrentGameState();

    auto postJumpInfo = changeGameState(jump);
    auto wasPieceCrowned = postJumpInfo.wasPieceCrowned;
    auto jumpDestination = postJumpInfo.spaceJumpedTo;

    game.swapPlayers();
    if (isBaseCase(depth)) {
        auto numPieces = game.activePlayer->baseCase(*this);
        setGameState(stateBeforeMove);

        return numPieces;
    }
    game.swapPlayers();

    EvaluationType bestNumPieces;

    auto multiJumps = game.getValidJumpsAt(jumpDestination);
    if (!wasPieceCrowned && multiJumps.size() > 0) {
        bestNumPieces = recurseMultiJumpCase(multiJumps, depth, alpha, beta);
    }
    else {
        game.swapPlayers();
        bestNumPieces = recursiveCase(depth, alpha, beta);
    }

    setGameState(stateBeforeMove);
    return bestNumPieces;
}

EvaluationType SearchHelper::recurseMultiJumpCase(
        const vector<JumpPackage> & multiJumps,
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    EvaluationType bestNumPieces = (isMaximizingPlayer) ? -INFINITY : INFINITY;
    EvaluationType jumpVal;

    if (isMaximizingPlayer) {
        for (auto & jump : multiJumps) {
            jumpVal = recurse(jump, depth, alpha, beta);

            bestNumPieces = max(jumpVal, bestNumPieces);
            alpha = max(alpha, bestNumPieces);

            if (beta <= alpha) {
                ++prunedNodes;
                break;
            }
        }
    } else {
        for (auto & jump : multiJumps) {
            jumpVal = recurse(jump, depth, alpha, beta);

            bestNumPieces = min(jumpVal, bestNumPieces);
            beta = min(beta, bestNumPieces);

            if (beta <= alpha) {
                ++prunedNodes;
                break;
            }
        }
    }

    return bestNumPieces;
}

#define SEARCH_ACTIONS(actions, cmpFunc)  \
    for (auto & jump : actions) {\
        bestVal = recurse(jump, depth, alpha, beta); \
\
        bestNumPieces = cmpFunc(bestVal, bestNumPieces);\
        alpha = cmpFunc(alpha, bestNumPieces);\
\
    \
        if (beta <= alpha) { \
            ++prunedNodes;\
            break;\
        }\
    }\

EvaluationType SearchHelper::recursiveCase(
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    EvaluationType bestNumPieces = (isMaximizingPlayer) ? -INFINITY : INFINITY;

    auto jumps = game.getValidJumps();

    if (jumps.size() != 0) {
        EvaluationType bestVal;

        if (isMaximizingPlayer) {
            SEARCH_ACTIONS(jumps, max);
        } else {
            SEARCH_ACTIONS(jumps, min);
        }
    }
    else {
        EvaluationType bestEval;
        if (isMaximizingPlayer) {
            for (auto & move : game.getValidMoves()) {
                bestEval = recurse(move, depth - 1, alpha, beta);

                bestNumPieces = max(bestEval, bestNumPieces);
                alpha = max(alpha, bestNumPieces);

                if (beta <= alpha) {
                    ++prunedNodes;
                    break;
                }
            }
        } else {
            for (auto & move : game.getValidMoves()) {
                bestEval = recurse(move, depth - 1, alpha, beta);

                bestNumPieces = min(bestEval, bestNumPieces);
                beta = min(beta, bestNumPieces);

                if (beta <= alpha) {
                    ++prunedNodes;
                    break;
                }
            }
        }
    }

    return bestNumPieces;
}


GameState SearchHelper::getCurrentGameState() {
    return GameState(
            game.board.getBoardState(),
            game.red->getPieces(),
            game.black->getPieces(),
            game.activePlayer->getColor()
            );
}

void SearchHelper::changeGameState(const MovePackage & move) {
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();
}

PostJumpInformation SearchHelper::changeGameState(const JumpPackage & jump) {
    game.board.make(jump);
    auto wasPieceCrowned = game.reactTo(jump);

    return PostJumpInformation(wasPieceCrowned, jump.second.to);
}

bool SearchHelper::isBaseCase(int depth) {
    return depth == 0 or !(game.areJumps() or game.areMoves());
}

void SearchHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
    game.activePlayer = gameState.activePlayerColor == 'r' ? game.red : game.black;
    game.inactivePlayer = gameState.activePlayerColor == 'r' ? game.black : game.red;
}
