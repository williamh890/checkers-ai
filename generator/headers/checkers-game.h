#include "models.h"
// ai::Piece
// ai::Position
#include "move-generator.h"
// ai::MoveGenerator

#include <memory>

namespace ai {
    class Player {
        private:
            char color;
            std::vector<Piece> pieces;
            MoveGenerator generator;

        public:
            Player(char color, MoveGenerator generator);
            std::vector<Piece> getPieces() const;
            char getColor() const;

            std::vector<int> getMovesFor(Piece piece) const;
            std::vector<Jump> getJumpsFor(Piece piece) const;
        private:
            void initPieces();
            virtual bool isInitialSpace(int space) const = 0;
    };

    class RedPlayer: public Player {
        public:
            RedPlayer(char color, MoveGenerator generator);
        private:
            bool isInitialSpace(int space) const override;
    };

    class BlackPlayer: public Player {
        public:
            BlackPlayer(char color, MoveGenerator generator);
        private:
            bool isInitialSpace(int space) const override;
    };

    class CheckersGame {
        public:
            std::shared_ptr<Player> red;
            std::shared_ptr<Player> black;

        public:
            CheckersGame(std::shared_ptr<Player> red, std::shared_ptr<Player> black);

        public:
            void printBoard();
            void printMoves();
            void printMovesForColor(const std::string & color);
            void printJumpsForColor(const std::string & color);

            void printValidMoves();
        private:
            std::vector<std::vector<char>> getEmptyBoard();
            Position spaceToGridSquare(int space);
    };

    CheckersGame getCheckersGame();
}
