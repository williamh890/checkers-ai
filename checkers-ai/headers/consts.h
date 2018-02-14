#ifndef CONSTS_H
#define CONSTS_H

#include "player.h"
// PlayerType

#include <string>

namespace ai {
    static const int INIT_NUM_PIECES = 12;
    static const int TOTAL_NUM_SPACES = 32;
    static const int NUM_PIECES_IN_ROW = 4;
    static const int ROWS = 8;
    static const int COLS = 8;
    static const int DEBUG = false;
    namespace Settings {
        static const std::string SEEDING_METHOD = "random_device"; // Options are random_device and time

        static const PlayerType RED_PLAYER_TYPE = PlayerType::Computer;
        static const PlayerType BLACK_PLAYER_TYPE = PlayerType::Computer;

        using NetworkWeightType = float;
    }
}

#endif
