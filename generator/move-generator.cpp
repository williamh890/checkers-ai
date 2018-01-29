#include "headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;
#include "headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <utility>
using std::pair;
using std::make_pair;
#include <stdio.h>

class MoveGenerator {
    private:
        MoveTableType moves;
        JumpTableType jumps;

    public:
        MoveGenerator(
                const MoveTableType & moves,
                const JumpTableType & jumps): moves(moves),
                                              jumps(jumps) {
        }

        auto getMoves(int space) {
            return this->moves[ space ];
        }

        auto getJumps(int space) {
            return this->jumps[ space ];
        }
};

auto getGeneratorFor(const string & color, const JsonToStlConverter & converter) {
    auto moves = converter.getMovesFor(color);
    auto jumps = converter.getJumpsFor(color);

    return MoveGenerator(moves, jumps);
}


struct Piece {
    char color;
    int space;

    Piece() = default;
    Piece(char color, int space): color(color), space(space)  {
    }
};

struct Position {
    int row;
    int col;
    Position(int r, int c): row(r), col(c) {}
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
        CheckersGame(
                MoveGenerator red,
                MoveGenerator black): redGenerator(red),
                                      blackGenerator(black) {
            this->initPieces();
        }

    private:
        /*
            +---+---+---+---+---+---+---+---+
            | X | r |   | r |   | r |   | r |
            +---+---+---+---+---+---+---+---+
            | r |   | r |   | r |   | r |   |
            +---+---+---+---+---+---+---+---+
            |   | r |   | r |   | r |   | r |
            +---+---+---+---+---+---+---+---+
            |   |   |   |   |   |   |   |   |
            +---+---+---+---+---+---+---+---+
            |   |   |   |   |   |   |   |   |
            +---+---+---+---+---+---+---+---+
            | b |   | b |   | b |   | b |   |
            +---+---+---+---+---+---+---+---+
            |   | b |   | b |   | b |   | b |
            +---+---+---+---+---+---+---+---+
            | b |   | b |   | b |   | b | X |
            +---+---+---+---+---+---+---+---+
        */

        void initPieces() {
            for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {

                if (space < (INIT_NUM_PIECES)) {
                    Piece newPiece = Piece('r', space);
                    this->redPieces.push_back(newPiece);
                }

                if (space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES)) {
                    Piece newPiece = Piece('b', space);
                    this->blackPieces.push_back(newPiece);
                }

            }
        }

    public:
        string printBoard() {
            string board = "";

            auto spaces = this->getEmptyBoard();

            for (auto piece: blackPieces) {
                auto pos = spaceToGridSquare(piece.space);
                spaces[pos.row][pos.col] = piece.color;
            }

            for (auto piece: redPieces) {
                auto pos = spaceToGridSquare(piece.space);
                spaces[pos.row][pos.col] = piece.color;
            }

            string spacerRow = "+---+---+---+---+---+---+---+---+";
            for ( auto row : spaces ) {
                cout << spacerRow << endl;

                for (auto space : row) {
                    cout << "| " << space << " ";
                }
                cout << "|" << endl;

            }
            cout << spacerRow << endl;

            return board;
        }

    private:
        vector<vector<char>> getEmptyBoard() {
            vector<vector<char>> board;

            for (auto r = 0; r < ROWS; ++r) {
                auto row = vector<char>(ROWS, ' ');
                board.push_back(row);
            }

            return board;
        }

    Position spaceToGridSquare(int space) {
        auto row = space / 4;
        auto col = space % 4;

        auto offset = (row % 2) ? 0 : 1;

        return Position(row, (2 * col) + offset);
    }

};

CheckersGame getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getGeneratorFor("black", converter);
    auto black = getGeneratorFor("red", converter);

    return CheckersGame(red, black);
}

/*
         +---+---+---+---+---+---+---+---+
         | X | 0 |   | 1 |   | 2 |   | 3 |
         +---+---+---+---+---+---+---+---+
         | 4 |   | 5 |   | 6 |   | 7 |   |
         +---+---+---+---+---+---+---+---+
         |   | 8 |   | 9 |   | 10|   | 11|
         +---+---+---+---+---+---+---+---+
         | 12|   | 13|   | 14|   | 15|   |
         +---+---+---+---+---+---+---+---+
         |   | 16|   | 17|   | 18|   | 19|
         +---+---+---+---+---+---+---+---+
         | 20|   | 21|   | 22|   | 23|   |
         +---+---+---+---+---+---+---+---+
         |   | 24|   | 25|   | 26|   | 27|
         +---+---+---+---+---+---+---+---+
         | 28|   | 29|   | 30|   | 31| X |
         +---+---+---+---+---+---+---+---+
*/

int main() {
    auto game = getCheckersGame();
    game.printBoard();

    return 0;
}

