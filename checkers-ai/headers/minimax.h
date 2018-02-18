// min_max.h

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
    int minimax(CheckersGame::MovePackage move, int depth, char maximizingPlayer, CheckersGame & game);
    int minimax(CheckersGame::JumpPackage jump, int depth, char maximizingPlayer, CheckersGame & game);

    using Pieces = std::vector<Piece>;
    using BoardState = std::vector<char>;

    struct GameState {
        BoardState boardState;
        Pieces redPieces;
        Pieces blackPieces;

        GameState(const BoardState & board, const Pieces & red, const Pieces & black);
    };

    class MiniMaxHelper {
        public:
            CheckersGame game;
            char maximizingPlayer;

            MiniMaxHelper(char maximizingPlayer, CheckersGame & game);

            int recurse(CheckersGame::MovePackage move, int depth);
            int recurse(CheckersGame::JumpPackage jump, int depth);

            GameState getCurrentGameState();
            void applyMoveTo(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);
            int handleBaseCase();
            int recursiveCase(int depth);

            void setGameState(GameState & gameState);
   };
};

#endif
