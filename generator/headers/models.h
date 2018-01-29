#ifndef MODELS_H
#define MODELS_H

namespace ai {
    struct Jump {
        int to;
        int from;

        Jump(int to, int from): to(to), from(from) {
        }
    };

    struct Piece {
        char color;
        int space;

        Piece(char color, int space): color(color), space(space) {
        }
    };

    struct Position {
        int row;
        int col;

        Position(int r, int c): row(r), col(c) {
        }
    };
}

#endif
