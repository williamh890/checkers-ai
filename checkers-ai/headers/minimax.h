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
    CheckersGame::MovePackage minimaxMove(CheckersGame & game, int depth);
    CheckersGame::JumpPackage minimaxJump(CheckersGame & game, int depth, int space=-1);

    int minimax(
            const CheckersGame::MovePackage & move,
            int depth,
            char maximizingPlayer,
            CheckersGame & game);
    int minimax(
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

    class MiniMaxHelper {
        public:
            static int totalNodes;
            static int prunedNodes;

            CheckersGame game;
            char maximizingPlayer;

            MiniMaxHelper() = default;
            MiniMaxHelper(char maximizingPlayer, CheckersGame & game);

            int recurse(const CheckersGame::MovePackage & move, int depth, int alpha, int beta);
            int recurse(const CheckersGame::JumpPackage & jump, int depth, int alpha, int beta);

            GameState getCurrentGameState();
            PostJumpInformation changeGameState(const CheckersGame::JumpPackage & jump);
            void changeGameState(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);
            int baseCase();

            bool isMultiJumpCase(
                    bool wasPieceCrowned,
                    const std::vector<CheckersGame::JumpPackage> & multiJumps
                    );
            int recurseMultiJumpCase(
                    const std::vector<CheckersGame::JumpPackage> & multiJumps,
                    int depth, int alpha, int beta
                    );
            int recursiveCase(int depth, int alpha, int beta);

            void setGameState(GameState & gameState);
    };
};

#endif
