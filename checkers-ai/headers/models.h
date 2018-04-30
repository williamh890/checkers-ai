#ifndef MODELS_H
#define MODELS_H

#include <sstream>
// std::stringstream
#include <string>
// std::string


namespace ai {
    struct Jump {
        int to;
        int through;
        Jump()= default;
        Jump(int to, int through): to(to), through(through) {
        }

        std::string toString() {
            std::stringstream s;
            s << "(" << to << ", " << through << ")";
            return s.str();
        }
        bool operator == (const Jump &rhs) const{
         return (to == rhs.to && through == rhs.through);
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

    struct PostJumpInformation {
        bool wasPieceCrowned;
        int spaceJumpedTo;

        PostJumpInformation(bool wasPieceCrowned, int spaceJumpedTo):
            wasPieceCrowned(wasPieceCrowned),
            spaceJumpedTo(spaceJumpedTo) {}
    };

    struct Piece {
        char color;
        int space;
        bool isKing = false;

        Piece(){}
        Piece(char color, int space): color(color), space(space) {
        }
        friend bool operator==(const Piece & rhs, const Piece & lhs) {
            return
                rhs.color == lhs.color and
                rhs.space == lhs.space and
                rhs.isKing == lhs.isKing;
        }
    };
} // namespace ai

#endif // MODELS_H
