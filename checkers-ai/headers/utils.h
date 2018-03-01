#ifndef UTILS_H
#define UTILS_H

#include "models.h"
// ai::Position

#include <string>
// std::string


namespace ai {
    Position spaceToPosition(int space);

    int positionToSpace(const Position & pos);

    std::string getUsername ();

    std::string idToFilename(int ID);

    double getTime();
}

#endif
