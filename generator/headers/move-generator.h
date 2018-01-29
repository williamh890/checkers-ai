#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "table-types.h"
#include "json-to-stl.h"

#include <string>

namespace ai {
    class MoveGenerator {
        private:
            ai::MoveTableType moves;
            ai::JumpTableType jumps;

        public:
            MoveGenerator(const ai::MoveTableType & moves, const ai::JumpTableType & jumps);

            auto getMoves(int space);
            auto getJumps(int space);
    };

    auto getGeneratorFor(const std::string & color, const ai::JsonToStlConverter & converter);

    struct Piece {
        char color;
        int space;

        Piece() = default;
        Piece(char color, int space);
    };

    struct Position {
        int row;
        int col;
        Position(int r, int c);
    };

    class CheckersGame {
        private:
            MoveGenerator redGenerator;
            MoveGenerator blackGenerator;

            vector<Piece> redPieces;
            vector<Piece> blackPieces;

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
            std::string printBoard();

        private:
            vector<vector<char>> getEmptyBoard();
            Position spaceToGridSquare(int space);
    };

    CheckersGame getCheckersGame();
}

#endif
