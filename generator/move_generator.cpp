#include "headers/json-to-stl.h"
#include "headers/table-types.h"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

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

int main() {
    auto game = getCheckersGame();
    return 0;
}

