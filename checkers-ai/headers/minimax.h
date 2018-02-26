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
    CheckersGame::JumpPackage minimaxJump(CheckersGame & game, int depth);

    int minimax(const CheckersGame::MovePackage & move, int depth, char maximizingPlayer, CheckersGame & game);
    int minimax(const CheckersGame::JumpPackage & jump, int depth, char maximizingPlayer, CheckersGame & game);

    using Pieces = std::vector<Piece>;
    using BoardState = std::vector<char>;

    struct GameState {
        BoardState boardState;
        Pieces redPieces;
        Pieces blackPieces;
        char activePlayerColor;
        GameState() = default;
        GameState(const BoardState & board, const Pieces & red, const Pieces & black, char activePlayerColor);
    };

    class MiniMaxHelper {
        public:
            static int totalNodes;

            CheckersGame game;
            char maximizingPlayer;

            MiniMaxHelper() = default;
            MiniMaxHelper(char maximizingPlayer, CheckersGame & game);

            int recurse(const CheckersGame::MovePackage & move, int depth);
            int recurse(const CheckersGame::JumpPackage & jump, int depth);

            GameState getCurrentGameState();
            int changeGameState(const CheckersGame::JumpPackage & jump);
            void changeGameState(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);
            int baseCase();

            int recurseMultiJumpCase(const std::vector<CheckersGame::JumpPackage> & multiJumps, int depth);
            int recursiveCase(int depth);

            void setGameState(GameState & gameState);
   };
};

#endif
