#include "models.h"
// ai::Piece
// ai::Position
#include "move-generator.h"
// ai::MoveGenerator


namespace ai {
    class CheckersGame {
        private:
            MoveGenerator redGenerator;
            MoveGenerator blackGenerator;

            std::vector<Piece> redPieces;
            std::vector<Piece> blackPieces;

            static const int INIT_NUM_PIECES = 12;
            static const int TOTAL_NUM_PIECES = 32;
            static const int ROWS = 8;
            static const int COLS = 8;

        public:
            CheckersGame();
            CheckersGame(MoveGenerator red, MoveGenerator black);
            //~CheckersGame();
        private:
            void initPieces();

            bool isInFirst3Rows(int space);
            bool isInLast3Rows(int space);

        public:
            void printBoard();
            void printMoves();
            void printMovesForColor(const std::string & color);
            void printJumpsForColor(const std::string & color);

            void printValidMoves();
        private:
            MoveTableType getBlackMoves();
            std::vector<std::vector<char>> getEmptyBoard();
            Position spaceToGridSquare(int space);
    };

    CheckersGame getCheckersGame();
}
