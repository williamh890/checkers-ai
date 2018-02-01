#include "headers/utils.h"
using ai::spaceToGridSquare;

#include "headers/models.h"
using ai::Position;

/*         0   1   2   3   4   5   6   7
         +---+---+---+---+---+---+---+---+
     0   | X | 0 |   | 1 |   | 2 |   | 3 |
         +---+---+---+---+---+---+---+---+
     1   | 4 |   | 5 |   | 6 |   | 7 |   |
         +---+---+---+---+---+---+---+---+
     2   |   | 8 |   | 9 |   | 10|   | 11|
         +---+---+---+---+---+---+---+---+
     3   | 12|   | 13|   | 14|   | 15|   |
         +---+---+---+---+---+---+---+---+
     4   |   | 16|   | 17|   | 18|   | 19|
         +---+---+---+---+---+---+---+---+
     5   | 20|   | 21|   | 22|   | 23|   |
         +---+---+---+---+---+---+---+---+
     6   |   | 24|   | 25|   | 26|   | 27|
         +---+---+---+---+---+---+---+---+
     7   | 28|   | 29|   | 30|   | 31| X |
         +---+---+---+---+---+---+---+---+
*/

Position ai::spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}


int ai::positionToSpace(const Position & pos) {
    int space = 0;
    auto offset = (pos.row % 2) ? 0 : 1;

    space += pos.row * 4;
    space += pos.col;

    space -= offset;
    return space;
}
