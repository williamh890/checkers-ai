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

            const int INIT_NUM_PIECES = 12;
            const int TOTAL_NUM_PIECES = 32;
            const int ROWS = 8;
            const int COLS = 8;

        public:
            CheckersGame(MoveGenerator red, MoveGenerator black);
        private:
            void initPieces();

            bool isInFirst3Rows(int space);
            bool isInLast3Rows(int space);

        public:
            void printBoard();
            void printMoves();
            void getMoves();
        private:
            MoveTableType getBlackMoves();
            std::vector<std::vector<char>> getEmptyBoard();
            Position spaceToGridSquare(int space);
    };

    CheckersGame getCheckersGame();
}
