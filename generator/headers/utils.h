#ifndef UTILS_H
#define UTILS_H

#include "models.h"
// ai::Position

namespace ai {
    Position spaceToGridSquare(int space);
    int positionToSpace(const Position & pos);
}

#endif
