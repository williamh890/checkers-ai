#ifndef MODELS_H
#define MODELS_H

#include <sstream>

namespace ai {
    struct Jump {
        int to;
        int through;

        Jump(int to, int through): to(to), through(through) {
        }

        std::string toString() {
            std::stringstream s;
            s << "(" << to << ", " << through << ")";
            return s.str();
        }
    };

    struct Piece {
        char color;
        int space;
        bool king = false;

        Piece(char color, int space): color(color), space(space) {
        }
    };

    struct Position {
        int row;
        int col;

        Position(int r, int c): row(r), col(c) {
        }

        std::string toString() {
            std::stringstream s;
            s << "[" << row << ", " << col << "]";
            return s.str();
        }
    };
}

#endif
