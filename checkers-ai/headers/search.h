#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "checkers-game.h"
// ai::CheckersGame

#include "game-state.h"
// ai::GameState

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

static const std::vector<char> COLORS = {'r', 'b'};
namespace ai {
    using EvaluationType = float;

    CheckersGame::MovePackage getBestMoveIDS(CheckersGame & game);
    CheckersGame::JumpPackage getBestJumpIDS(CheckersGame & game, int space=-1);

    CheckersGame::MovePackage getBestMove(CheckersGame & game, int depth);
    CheckersGame::JumpPackage getBestJump(CheckersGame & game, int depth, int space);

    template<typename ActionType>
    EvaluationType search(
            const ActionType & action,
            int depth,
            char maximizingPlayer,
            CheckersGame & game);

    class SearchHelper {
        public:
            static int totalNodes;
            static int prunedNodes;
            static int leafNodes;
            static double searchStartTime;
            static double limit;

            CheckersGame game;
            char maximizingPlayer;

            SearchHelper() = default;
            SearchHelper(char maximizingPlayer, CheckersGame & game);

            EvaluationType recurse(
                    const CheckersGame::MovePackage & move,
                    int depth,
                    EvaluationType alpha,
                    EvaluationType beta);
            EvaluationType recurse(
                    const CheckersGame::JumpPackage & jump,
                    int depth,
                    EvaluationType alpha,
                    EvaluationType beta);

            bool isOutOfTime();
            GameState getCurrentGameState();

            PostJumpInformation changeGameState(const CheckersGame::JumpPackage & jump);
            void changeGameState(const CheckersGame::MovePackage & move);
            bool isBaseCase(int depth);

            bool isMultiJumpCase(
                    bool wasPieceCrowned,
                    const std::vector<CheckersGame::JumpPackage> & multiJumps);

            EvaluationType recurseMultiJumpCase(
                    const std::vector<CheckersGame::JumpPackage> & multiJumps,
                    int depth,
                    EvaluationType alpha,
                    EvaluationType beta);

            EvaluationType recursiveCase(
                    int depth,
                    EvaluationType alpha,
                    EvaluationType beta);

            void setGameState(GameState & gameState);
    };
};

template<typename ActionType>
ai::EvaluationType ai::search(
        const ActionType & action,
        int depth,
        char maximizingPlayer,
        CheckersGame & game) {
    SearchHelper search(maximizingPlayer, game);

    EvaluationType alpha=-INFINITY, beta=INFINITY;
    return search.recurse(action, depth, alpha, beta);
}

#endif
