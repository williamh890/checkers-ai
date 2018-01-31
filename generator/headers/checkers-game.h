#include "models.h"
// ai::Piece
// ai::Position
#include "move-generator.h"
// ai::MoveGenerator
#include "player.h"
// ai::Player

#include <memory>

namespace ai {
    class CheckersGame {
        private:
            std::vector<char> board;
        public:
            std::shared_ptr<Player> red;
            std::shared_ptr<Player> black;

        public:
            CheckersGame() = delete;
            CheckersGame(std::shared_ptr<Player> red, std::shared_ptr<Player> black);
        private:
            void addPiecesToBoardFor(std::shared_ptr<Player> player);

        public:
            void printBoard();
            void printMoves();

            void printMovesForColor(const std::string & color);
            void printJumpsForColor(const std::string & color);

            // void printValidMoves(); This is not defined in the source yet
        private:
            MoveTableType getBlackMoves();
            std::vector<std::vector<char>> getEmptyBoard();
            Position spaceToGridSquare(int space);
    };

    CheckersGame getCheckersGame();
}
