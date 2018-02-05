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

        bool operator==(const Jump & other) const {
            return (this->to == other.to && this->through == other.through);
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

    struct Piece {
        char color;
        int space;
        bool isKing = false;

        Piece(char color, int space): color(color), space(space) {
        }
    };
}

#endif
