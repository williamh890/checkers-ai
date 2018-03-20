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
    using MiniMaxReturnType = float;

    CheckersGame::MovePackage minimaxMove(CheckersGame & game, int depth);
    CheckersGame::JumpPackage minimaxJump(CheckersGame & game, int depth, int space=-1);

    MiniMaxReturnType minimax(
            const CheckersGame::MovePackage & move,
            int depth,
            char maximizingPlayer,
            CheckersGame & game);

    MiniMaxReturnType minimax(
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

            MiniMaxReturnType recurse(const CheckersGame::MovePackage & move, int depth, MiniMaxReturnType alpha, MiniMaxReturnType beta);
            MiniMaxReturnType recurse(const CheckersGame::JumpPackage & jump, int depth, MiniMaxReturnType alpha, MiniMaxReturnType beta);

            GameState getCurrentGameState();
            PostJumpInformation changeGameState(const CheckersGame::JumpPackage & jump);
            void changeGameState(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);

            bool isMultiJumpCase(
                    bool wasPieceCrowned,
                    const std::vector<CheckersGame::JumpPackage> & multiJumps
                    );
            MiniMaxReturnType recurseMultiJumpCase(
                    const std::vector<CheckersGame::JumpPackage> & multiJumps,
                    int depth, MiniMaxReturnType alpha, MiniMaxReturnType beta
                    );
            MiniMaxReturnType recursiveCase(int depth, MiniMaxReturnType alpha, MiniMaxReturnType beta);

            void setGameState(GameState & gameState);
    };
};

#endif
