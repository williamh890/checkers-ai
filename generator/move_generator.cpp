#include "headers/json-to-stl.h"
#include "headers/table-types.h"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <cstddef>
using std::pair;
using std::make_pair;

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
            return this->moves.at( space );
        }

        auto getJumps(int space) {
            return this->jumps.at( space );
        }
};

auto getGeneratorFor(const string & color, const JsonToStlConverter & converter) {
    auto moves = converter.getMovesFor(color);
    auto jumps = converter.getJumpsFor(color);

    return MoveGenerator(moves, jumps);
}

enum class Color { Red, Black };

struct Piece {
    Color color;
    int space;

    Piece(Color color, int space): color(color), space(space)  {
    }
};

class CheckersGame {
    private:
        MoveGenerator redGenerator;
        MoveGenerator blackGenerator;

        vector<Piece> pieces;

    public:
        CheckersGame(
                MoveGenerator red,
                MoveGenerator black): redGenerator(red),
                                      blackGenerator(black) {
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
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    | X | r |   | r |   | r |   | r |     | X | 0 |   | 1 |   | 2 |   | 3 |
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    | r |   | r |   | r |   | r |   |     | 4 |   | 5 |   | 6 |   | 7 |   |
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    |   | r |   | r |   | r |   |   |     |   | 8 |   | 9 |   | 10|   | 11|
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |     | 12|   | 13|   | 14|   | 15|   |
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |     |   | 16|   | 17|   | 18|   | 19|
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    | b |   | b |   | b |   | b |   |     | 20|   | 21|   | 22|   | 23|   |
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    |   | b |   | b |   | b |   | b |     |   | 24|   | 25|   | 26|   | 27|
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+
    | b |   | b |   | b |   | b | X |     | 28|   | 29|   | 30|   | 31| X |
    +---+---+---+---+---+---+---+---+     +---+---+---+---+---+---+---+---+


*/
struct Position {
    int row;
    int col;
    Position(int r, int c): row(r), col(c) {}
};

Position spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}

string boardToString(const vector<Piece> pieces) {
    return "";
}

int main() {
    // auto game = getCheckersGame();
    for (auto i = 0; i < 32; ++i) {
        auto space = spaceToGridSquare(i);
        cout << "Space (" << i << ") = "
            << "r: " << space.row << ", c: " << space.col
            << endl;
    }

    return 0;
}

