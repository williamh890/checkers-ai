#include "search.h"
using ai::EvaluationType;
using ai::search;
using ai::SearchHelper;

#include "game-state.h"
using ai::GameState;

#include "models.h"
using ai::Piece;
using ai::Jump;
using ai::PostJumpInformation;

#include "checkers-game.h"
using ai::CheckersGame;
using JumpPackage = CheckersGame::JumpPackage;
using MovePackage = CheckersGame::MovePackage;

#include "utils.h"
using ai::getTime;

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
using std::out_of_range;

using Pieces = vector<Piece>;
using BoardState = vector<char>;

int SearchHelper::totalNodes = 0;
int SearchHelper::prunedNodes = 0;
int SearchHelper::leafNodes = 0;
double SearchHelper::searchStartTime = 0.;
double SearchHelper::limit = 0.;

CheckersGame::MovePackage ai::getBestMoveIDS(CheckersGame & game) {
    int depth = 3;
    CheckersGame::MovePackage bestMove;
    SearchHelper::searchStartTime = getTime();

    auto stateBeforeSearch = game.getState();

    while (depth++) {
        try {
            bestMove = ai::getBestMove(game, depth);
        } catch(...) {
            cout << "done" << endl;
            break;
        }
        cout << "iterating: " << depth << endl;
    }

    game.setState(stateBeforeSearch);

    return bestMove;
}

CheckersGame::JumpPackage ai::getBestJumpIDS(CheckersGame & game, int space) {
    SearchHelper::searchStartTime = 0.;
    int depth = 3;
    CheckersGame::JumpPackage bestJump;
    SearchHelper::searchStartTime = getTime();

    auto stateBeforeSearch = game.getState();
    while (depth++ && depth < 100) {
        try {
            bestJump = ai::getBestJump(game, depth, space);
        } catch(out_of_range & err) {
            cout << "done" << endl;
            break;
        }
        cout << "iterating: " << depth << endl;
    }

    game.setState(stateBeforeSearch);

    return bestJump;
}


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

#ifdef USE_IDS
    if (isOutOfTime()) {
        throw out_of_range("out of time!");
    }
#endif

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

#ifdef USE_IDS
    if (isOutOfTime()) {
        throw out_of_range("out of time!");
    }
#endif

    auto stateBeforeMove = getCurrentGameState();
    auto jmpInfo = changeGameState(jump);

    game.swapPlayers();
    if (isBaseCase(depth)) {
        auto numPieces = game.activePlayer->baseCase(*this);
        setGameState(stateBeforeMove);

        return numPieces;
    }
    game.swapPlayers();

    EvaluationType bestOverallValue;

    auto multiJumps = game.getValidJumpsAt(jmpInfo.spaceJumpedTo);
    if (!jmpInfo.wasPieceCrowned && multiJumps.size() > 0) {
        bestOverallValue = recurseMultiJumpCase(multiJumps, depth, alpha, beta);
    }
    else {
        game.swapPlayers();
        bestOverallValue = recursiveCase(depth, alpha, beta);
    }

    setGameState(stateBeforeMove);
    return bestOverallValue;
}

bool SearchHelper::isOutOfTime() {
    return (getTime() - SearchHelper::searchStartTime) > SearchHelper::limit;
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

#define SEARCH_BASED_ON_MAXIMIZING_PLAYER(actions, depthExpression)                  \
    if (isMaximizingPlayer) {                                                        \
        SEARCH_ACTIONS(actions, depthExpression, max, alpha);                        \
    } else {                                                                         \
        SEARCH_ACTIONS(actions, depthExpression, min, beta);                         \
    }                                                                                \

EvaluationType SearchHelper::recurseMultiJumpCase(
        const vector<JumpPackage> & multiJumps,
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {

    SETUP_SEACH_VARIABLES()
    SEARCH_BASED_ON_MAXIMIZING_PLAYER(multiJumps, depth)

    return bestOverallVal;
}

EvaluationType SearchHelper::recursiveCase(
        int depth,
        EvaluationType alpha,
        EvaluationType beta) {

    SETUP_SEACH_VARIABLES()
    auto jumps = game.getValidJumps();

    if (jumps.size() != 0) {
        SEARCH_BASED_ON_MAXIMIZING_PLAYER(jumps, depth)
    } else {
        auto moves = game.getValidMoves();
        SEARCH_BASED_ON_MAXIMIZING_PLAYER(moves, depth-1)
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
