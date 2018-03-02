#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "board.h"
// ai::Board
#include "move-generator.h"
// ai::MoveGenerator
#include "seeder.h"
// ai::Seeder
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
        public:
            using MovePackage = std::pair<int, int>;
            using JumpPackage = std::pair<int, Jump>;
            using PlayerPtr = std::shared_ptr<Player>;
            using SeederPtr = std::shared_ptr<Seeder>;

            using turn_type = std::pair<Board::BoardType, std::pair<std::vector<MovePackage>, std::vector<JumpPackage>>>;

            static int MINIMAX_SEARCH_DEPTH;
            int moveCounter = 0;

            Board board;

            PlayerPtr red;
            PlayerPtr black;

            PlayerPtr activePlayer;
            PlayerPtr inactivePlayer;

            std::vector<std::vector<int>> game_record{};

            std::mt19937 generator;

        public:
            CheckersGame();
            CheckersGame(
                    const Board & board,
                    PlayerPtr red,
                    PlayerPtr black,
                    SeederPtr & seeder
                    );

            void makeRandomValidAction();
            void makeMinimaxMove(int depth);

            bool isInvalid(const MovePackage & move);
            bool isInvalid(const JumpPackage & jump);

            const char getActivePlayerColor();
            const char getInactivePlayerColor();

            void makeJump(const JumpPackage & jump);
            void makeMove(const MovePackage & move);

            void replayJump(const JumpPackage & jump);
            void replayMove(const MovePackage & move);

            void swapPlayers();
            bool areJumps();
            bool areMoves();

            std::vector<std::vector<int>> getGame();
            // end gui/cython wrapper functions

            std::vector<MovePackage> getValidMoves();
            std::vector<JumpPackage> getValidJumps();
            std::vector<JumpPackage> getValidJumpsAt(int space);

            std::vector<JumpPackage> getOpponentValidJumps();
            std::vector<MovePackage> getOpponentValidMoves();

            void play();

            MovePackage getMoveFromActivePlayer();

            MovePackage getMoveFromUser();
            MovePackage getRandomValidMove();
            MovePackage getMinimaxMove();

            JumpPackage getJumpFromActivePlayer();

            JumpPackage getJumpFromUser();
            JumpPackage getRandomValidJump();
            JumpPackage getMinimaxJump(int space=-1);

            MovePackage parseUserInput();

            JumpPackage getJumpFrom(const MovePackage & inputJump);

            void reactTo(const MovePackage & move);
            void reactTo(const JumpPackage & jump);

            std::string toString();

            int getNumPiecesFor(char color);
    };

    CheckersGame getCheckersGame();
}
#endif
