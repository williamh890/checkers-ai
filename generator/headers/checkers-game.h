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

#include <iostream>
// std::cout;
// std::endl;
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

            template <typename U>
            std::vector<std::pair<std::vector<char>, U>> Turn(const std::pair<int, int> & move);

            template <typename U>
            std::vector<std::pair<std::vector<char>, U>> Turn(const std::pair<int, Jump> & jump);
            // end gui/cython wrapper functions
            void play();

        private:
            //helper functions for exposing to cython
            template <typename U>
            std::vector<std::pair<std::vector<char>, U>> getAction();
            // end
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

    template <class U>
        std::vector<std::pair<std::vector<char>, U>>
        CheckersGame::Turn(const std::pair<int, int> & move) {

            if (isInvalid(move)){
                std::cout << "invalid move" << std::endl;
                return make_pair(getBoard(), getValidMoves());
            }

            auto action = board.make(move);
            reactTo(action, move);
            swapPlayers();

            auto validJumps = getValidJumps();

            if (validJumps.size()) {
                std::cout << activePlayer->getColor() << " has a jump" << std::endl;
                return make_pair(getBoard(), validJumps);
            }

            auto validMoves = getValidMoves();
            if (validMoves.size()) {
                std::cout << activePlayer->getColor() << " has a move" << std::endl;
                return make_pair(getBoard(), getValidMoves());
            }

            std::cout << inactivePlayer->getColor() << " wins" << std::endl;
        }

    template <class U>
        std::vector<std::pair<std::vector<char>, U>>
        CheckersGame::Turn(const std::pair<int, Jump> & jump) {

            if (isInvalid(jump)){
                std::cout << activePlayer->getColor() << " made an invalid jump" << std::endl;

                return make_pair(getBoard(), getValidJumps());
            }

            auto action = board.make(jump);
            reactTo(action, jump);

            auto validJumps = getValidJumpsAt(jump.second.to);

            if (validJumps.size()){
                std::cout << activePlayer->getColor() << " has another jump" << std::endl;
                return make_pair(getBoard(), validJumps);
            }

            swapPlayers();
            validJumps = getValidJumps();

            if (validJumps.size()){
                std::cout << activePlayer->getColor() << " has a jump" << std::endl;
                return make_pair(getBoard(), validJumps);
            }

            return make_pair(getBoard(), getValidMoves());
        }


    CheckersGame getCheckersGame();
}

#endif
