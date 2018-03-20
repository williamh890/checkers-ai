#include "headers/minimax.h"
using ai::MiniMaxReturnType;
using ai::minimax;
using ai::MiniMaxHelper;
using ai::GameState;

#include "headers/models.h"
using ai::Piece;
using ai::Jump;
using ai::PostJumpInformation;

#include "headers/checkers-game.h"
using ai::CheckersGame;
using JumpPackage = CheckersGame::JumpPackage;
using MovePackage = CheckersGame::MovePackage;

#include <climits>
#include <cfloat>
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
int MiniMaxHelper::prunedNodes = 0;

CheckersGame::MovePackage ai::minimaxMove(CheckersGame & game, int depth) {
    MiniMaxReturnType bestMoveVal = -FLT_MAX;
    MovePackage bestMove;

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto moves = game.getValidMoves();
    MovePackage move;
    MiniMaxReturnType moveVal;

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


CheckersGame::JumpPackage ai::minimaxJump(CheckersGame & game, int depth, int space) {
    MiniMaxReturnType bestJumpVal = -FLT_MAX;

    JumpPackage bestJump;

    if (depth == 0) {
        throw invalid_argument("Search depth must be > 0");
    }

    auto jumps = space == -1 ? game.getValidJumps() : game.getValidJumpsAt(space);
    JumpPackage jump;
    MiniMaxReturnType jumpVal;

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


MiniMaxReturnType ai::minimax(
        const MovePackage & move,
        int depth,
        char maximizingPlayer,
        CheckersGame & game) {
    MiniMaxHelper minimax(maximizingPlayer, game);

    MiniMaxReturnType alpha=-FLT_MAX, beta=FLT_MAX;
    return minimax.recurse(move, depth, alpha, beta);
}

MiniMaxReturnType ai::minimax(
        const JumpPackage & jump,
        int depth,
        char maximizingPlayer,
        CheckersGame & game) {

    MiniMaxHelper minimax(maximizingPlayer, game);

    MiniMaxReturnType alpha=-FLT_MAX, beta=FLT_MAX;
    return minimax.recurse(jump, depth, alpha, beta);
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

MiniMaxReturnType MiniMaxHelper::recurse(
        const MovePackage & move,
        int depth,
        MiniMaxReturnType alpha,
        MiniMaxReturnType beta) {
    ++totalNodes;
    auto stateBeforeMove = getCurrentGameState();

    changeGameState(move);

    MiniMaxReturnType bestNumPieces = isBaseCase(depth) ?
        game.activePlayer->baseCase(*this) :
        recursiveCase(depth, alpha, beta);

    setGameState(stateBeforeMove);

    return bestNumPieces;
}

MiniMaxReturnType MiniMaxHelper::recurse(
        const JumpPackage & jump,
        int depth,
        MiniMaxReturnType alpha,
        MiniMaxReturnType beta) {
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

    MiniMaxReturnType bestNumPieces;

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

MiniMaxReturnType MiniMaxHelper::recurseMultiJumpCase(
        const vector<JumpPackage> & multiJumps,
        int depth,
        MiniMaxReturnType alpha,
        MiniMaxReturnType beta) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    MiniMaxReturnType bestNumPieces = (isMaximizingPlayer) ? -FLT_MAX : FLT_MAX;
    MiniMaxReturnType jumpVal;

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

MiniMaxReturnType MiniMaxHelper::recursiveCase(
        int depth,
        MiniMaxReturnType alpha,
        MiniMaxReturnType beta) {
    auto isMaximizingPlayer = game.activePlayer->getColor() == maximizingPlayer;
    MiniMaxReturnType bestNumPieces = (isMaximizingPlayer) ? -FLT_MAX : FLT_MAX;

    auto jumps = game.getValidJumps();

    if (jumps.size() != 0) {
        MiniMaxReturnType jumpVal;

        if (isMaximizingPlayer) {
            for (auto & jump : jumps) {
                jumpVal = recurse(jump, depth, alpha, beta);

                bestNumPieces = max(jumpVal, bestNumPieces);
                alpha = max(alpha, bestNumPieces);

                if (beta <= alpha) {
                    ++prunedNodes;
                    break;
                }
            }
        } else {
            for (auto & jump : jumps) {
                jumpVal = recurse(jump, depth, alpha, beta);

                bestNumPieces = min(jumpVal, bestNumPieces);
                beta = min(beta, bestNumPieces);

                if (beta <= alpha) {
                    ++prunedNodes;
                    break;
                }
            }
        }
    }
    else {
        MiniMaxReturnType moveVal;
        if (isMaximizingPlayer) {
            for (auto & move : game.getValidMoves()) {
                moveVal = recurse(move, depth - 1, alpha, beta);

                bestNumPieces = max(moveVal, bestNumPieces);
                alpha = max(alpha, bestNumPieces);
                if (beta <= alpha) {
                    ++prunedNodes;
                    break;
                }
            }
        } else {
            for (auto & move : game.getValidMoves()) {
                moveVal = recurse(move, depth - 1, alpha, beta);

                bestNumPieces = min(moveVal, bestNumPieces);
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

PostJumpInformation MiniMaxHelper::changeGameState(const JumpPackage & jump) {
    game.board.make(jump);
    auto wasPieceCrowned = game.reactTo(jump);

    return PostJumpInformation(wasPieceCrowned, jump.second.to);
}

bool MiniMaxHelper::isBaseCase(int depth) {
    return depth == 0 or !(game.areJumps() or game.areMoves());
}

void MiniMaxHelper::setGameState(GameState & gameState) {
    game.board.setBoardState(gameState.boardState);
    game.red->setPieces(gameState.redPieces);
    game.black->setPieces(gameState.blackPieces);
    game.activePlayer = gameState.activePlayerColor == 'r' ? game.red : game.black;
    game.inactivePlayer = gameState.activePlayerColor == 'r' ? game.black : game.red;
}
