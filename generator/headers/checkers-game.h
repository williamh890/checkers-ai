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

namespace ai {
    class CheckersGame {
        private:
            Board board;
            std::shared_ptr<Player> red;
            std::shared_ptr<Player> black;

        public:
            CheckersGame() = delete;
            CheckersGame(const Board & board, std::shared_ptr<Player> red, std::shared_ptr<Player> black);
        private:

        public:
            std::string toString();
            void printMoves();

            void printMovesForColor(const std::string & color);
            void printJumpsForColor(const std::string & color);

            // void printValidMoves(); This is not defined in the source yet
        private:
            MoveTableType getBlackMoves();
    };

    CheckersGame getCheckersGame();
}

#endif
