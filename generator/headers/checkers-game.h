#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "board.h"
// ai::Board
#include "move-generator.h"
// ai::MoveGenerator
#include "player.h"
// ai::Player
#include "models.h"
// ai::Piece
// ai::Position

#include <memory>
#include <vector>
#include <utility>

namespace ai {
    class CheckersGame {
        private:
            Board board;
            std::shared_ptr<Player> red;
            std::shared_ptr<Player> black;

            std::shared_ptr<Player> activePlayer;

        public:
            CheckersGame();
            CheckersGame(const Board & board, std::shared_ptr<Player> red, std::shared_ptr<Player> black);

            void play();

        private:
            std::pair<int, int> getMoveFromActivePlayer();
            std::pair<int, int> getRandomValidMove();
            std::pair<int, int> getMoveFromUser();

            std::vector<std::pair<int, int>> getValidMoves();
            void reactTo(const Action & action);

            std::string toString();

            // void printValidMoves(); This is not defined in the source yet
        private:
            MoveTableType getBlackMoves();
            MoveTableType getRedMoves();
    };

    CheckersGame getCheckersGame();
}

#endif
