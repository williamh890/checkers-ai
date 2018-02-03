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
// std::shared_ptr
#include <vector>
// std::vector
#include <utility>
// std::pair
#include <random>
// std::mt19937

namespace ai {
    class CheckersGame {
        private:
            int moveCounter = 0;

            Board board;
            std::shared_ptr<Player> red;
            std::shared_ptr<Player> black;

            std::shared_ptr<Player> activePlayer;
            std::shared_ptr<Player> inactivePlayer;

            std::mt19937 generator;

        public:
            CheckersGame();
            CheckersGame(const Board & board, std::shared_ptr<Player> red, std::shared_ptr<Player> black);
            // following are public for use in the gui/cython wrapper
            bool isInvalid(const std::pair<int, int> & move);
            bool isInvalid(const std::pair<int, Jump> & jump);
            std::vector<char> getBoard(); // may not need
            const char getActivePlayerColor();
            void swapPlayers(); // may not need

            template <class U>
            std::vector<std::pair<std::vector<char>, U>> Turn(const std::pair<int, int> & move);

            template <class U>
            std::vector<std::pair<std::vector<char>, U>> Turn(const std::pair<int, Jump> & jump);
            // end gui/cython wrapper functions
            void play();

        private:
            std::pair<int, int> getMoveFromActivePlayer();
            std::pair<int, int> getRandomValidMove();
            std::pair<int, int> getMoveFromUser();

            std::pair<int, Jump> getJumpFromActivePlayer();
            std::pair<int, Jump> getRandomValidJump();
            std::pair<int, Jump> getJumpFromUser();

            std::pair<int, int> parseUserInput();

            std::pair<int, Jump> getJumpFrom(const std::pair<int, int> & inputJump);

            std::vector<std::pair<int, Jump>> getValidJumps();
            std::vector<std::pair<int, Jump>> getValidJumpsAt(int space);

            std::vector<std::pair<int, int>> getValidMoves();

            void reactTo(const Action & action, const std::pair<int, int> & move);
            void reactTo(const Action & action, const std::pair<int, Jump> & jump);

            std::string toString();

            // void printValidMoves(); This is not defined in the source yet
        private:
            MoveTableType getBlackMoves();
            MoveTableType getRedMoves();
    };

    CheckersGame getCheckersGame();
}

#endif
