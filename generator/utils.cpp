#include "headers/utils.h"
using ai::spaceToGridSquare;

#include "headers/models.h"
using ai::Position;

Position ai::spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}
