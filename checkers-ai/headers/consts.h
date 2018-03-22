#ifndef CONSTS_H
#define CONSTS_H

#include <string>
// std::string
#include <vector>
// std::vector
#include <cfloat>
// FLT_MAX

namespace ai {
    enum class PlayerType {
        Human, Computer
    };
    static int blondiePopulation = 30;
    static std::vector<unsigned int> blondieDimensions{32, 40, 10, 1};

    static const int INIT_NUM_PIECES = 12;
    static const int TOTAL_NUM_SPACES = 32;
    static const int NUM_PIECES_IN_ROW = 4;
    static const int ROWS = 8;
    static const int COLS = 8;
    static const int MINIMAX_SEARCH_DEPTH = 8;
    static const int DEBUG = false;
    static const int MOVE_LIMIT = 100;

    static const unsigned int NETWORKPOPSIZE = blondiePopulation;

    const float INF = FLT_MAX;

    static const std::vector<unsigned int> NETWORK_DIMENSIONS = blondieDimensions;

    namespace Settings {
        static const std::string SEEDING_METHOD = "random_device"; // Options are random_device and time

        static const PlayerType RED_PLAYER_TYPE = PlayerType::Computer;
        static const PlayerType BLACK_PLAYER_TYPE = PlayerType::Computer;

        using NetworkWeightType = float;
    }
}

#endif
