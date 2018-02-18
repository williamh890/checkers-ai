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
    int minimax(std::pair<int, int> move, int depth, char maximizingPlayer, CheckersGame & game);

    using Pieces = std::vector<Piece>;
    using BoardState = std::vector<char>;

    struct GameState {
        BoardState boardState;
        std::vector<Piece> redPieces;
        std::vector<Piece> blackPieces;

        GameState(const BoardState & board, const Pieces & red, const Pieces & black);
    };

    class MiniMaxHelper {
        public:
            CheckersGame game;
            char maximizingPlayer;

            MiniMaxHelper(char maximizingPlayer, CheckersGame & game);

            int recurse(CheckersGame::MovePackage move, int depth);

            GameState getCurrentGameState();
            void applyMoveTo(const CheckersGame::MovePackage & move);

            bool isBaseCase(int depth);
            int handleBaseCase();
            int recursiveCase(int depth);

            void setGameState(GameState & gameState);
   };
};

#endif
