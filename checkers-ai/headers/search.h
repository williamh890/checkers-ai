#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "checkers-game.h"
// ai::CheckersGame

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

static const std::vector<char> COLORS = {'r', 'b'};
namespace ai {
    using EvaluationType = float;

    CheckersGame::MovePackage getBestMove(CheckersGame & game, int depth);
    CheckersGame::JumpPackage getBestJump(CheckersGame & game, int depth, int space=-1);

    EvaluationType search(
            const CheckersGame::MovePackage & move,
            int depth,
            char maximizingPlayer,
            CheckersGame & game);

    EvaluationType search(
            const CheckersGame::JumpPackage & jump,
            int depth,
            char maximizingPlayer,
            CheckersGame & game);

    using Pieces = std::vector<Piece>;
    using BoardState = std::vector<char>;

    struct GameState {
        BoardState boardState;
        Pieces redPieces;
        Pieces blackPieces;
        char activePlayerColor;

        GameState() = default;
        GameState(
            const BoardState & board,
            const Pieces & red,
            const Pieces & black,
            char activePlayerColor);
    };

    class SearchHelper {
        public:
            static int totalNodes;
            static int prunedNodes;
            static int leafNodes;

            CheckersGame game;
            char maximizingPlayer;

            SearchHelper() = default;
            SearchHelper(char maximizingPlayer, CheckersGame & game);

            EvaluationType recurse(const CheckersGame::MovePackage & move, int depth, EvaluationType alpha, EvaluationType beta);
            EvaluationType recurse(const CheckersGame::JumpPackage & jump, int depth, EvaluationType alpha, EvaluationType beta);

            GameState getCurrentGameState();
            PostJumpInformation changeGameState(const CheckersGame::JumpPackage & jump);
            void changeGameState(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);

            bool isMultiJumpCase(
                    bool wasPieceCrowned,
                    const std::vector<CheckersGame::JumpPackage> & multiJumps
                    );
            EvaluationType recurseMultiJumpCase(
                    const std::vector<CheckersGame::JumpPackage> & multiJumps,
                    int depth, EvaluationType alpha, EvaluationType beta
                    );
            EvaluationType recursiveCase(int depth, EvaluationType alpha, EvaluationType beta);

            void setGameState(GameState & gameState);
    };
};

#endif
