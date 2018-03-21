#include "headers/search.h"
using ai::EvaluationType;
using ai::search;
using ai::SearchHelper;

#include "headers/game-state.h"
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
    EvaluationType bestMoveVal = -INF;
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
    EvaluationType bestJumpVal = -INF;

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

SearchHelper::SearchHelper(char maximizingPlayer, CheckersGame & game) :
    game(game),
    maximizingPlayer(maximizingPlayer) {
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

    EvaluationType bestOverallValue;

    auto multiJumps = game.getValidJumpsAt(jumpDestination);
    if (!wasPieceCrowned && multiJumps.size() > 0) {
        bestOverallValue = recurseMultiJumpCase(multiJumps, depth, alpha, beta);
    }
    else {
        game.swapPlayers();
        bestOverallValue = recursiveCase(depth, alpha, beta);
    }

    setGameState(stateBeforeMove);
    return bestOverallValue;
}

#define SETUP_SEACH_VARIABLES()                                                      \
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;     \
    EvaluationType bestOverallVal = (isMaximizingPlayer) ? -INF : INF;     \
    EvaluationType bestVal;\

#define SEARCH_ACTIONS(actions, depthExpression, cmpFunc, toUpdate)                  \
    for (auto & action : actions) {                                                  \
        bestVal = recurse(action, depthExpression, alpha, beta);                     \
                                                                                     \
        bestOverallVal = cmpFunc(bestVal, bestOverallVal);                           \
        toUpdate = cmpFunc(toUpdate, bestOverallVal);                                \
                                                                                     \
        if (beta <= alpha) {                                                         \
            ++prunedNodes;                                                           \
            break;                                                                   \
        }                                                                            \
    }                                                                                \

EvaluationType SearchHelper::recurseMultiJumpCase(
        const vector<JumpPackage> & multiJumps,
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    SETUP_SEACH_VARIABLES()

    if (isMaximizingPlayer) {
        SEARCH_ACTIONS(multiJumps, depth, max, alpha);
    } else {
        SEARCH_ACTIONS(multiJumps, depth, min, beta);
    }

    return bestOverallVal;
}

EvaluationType SearchHelper::recursiveCase(
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {
    SETUP_SEACH_VARIABLES()

    auto jumps = game.getValidJumps();

    if (jumps.size() != 0) {
        if (isMaximizingPlayer) {
            SEARCH_ACTIONS(jumps, depth, max, alpha);
        } else {
            SEARCH_ACTIONS(jumps, depth, min, beta);
        }
    }
    else {
        auto moves = game.getValidMoves();

        if (isMaximizingPlayer) {
            SEARCH_ACTIONS(moves, depth-1, max, alpha);
        } else {
            SEARCH_ACTIONS(moves, depth-1, min, beta);
        }
    }

    return bestOverallVal;
}


GameState SearchHelper::getCurrentGameState() {
    return game.getState();
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

void SearchHelper::setGameState(GameState & state) {
    game.setState(state);
}
