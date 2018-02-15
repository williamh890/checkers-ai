// min_max.h

#ifndef MIN_MAX_H
#define MIN_MAX_H
#include "network.h"
// ai::Network

#include "checkers-game.h"
// ai::CheckersGame

#include "table-types.h"
// ai::MoveTableType
// ai::JumpTableType

#include "move-generator.h"
// ai::MoveGenerator

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

namespace ai{
    class MinMaxHelper{
        public:
            using BoardType = std::vector<char>;
            MinMaxHelper() = default;
            MinMaxHelper(BoardType board, const std::string color); // color is the Player
                                                                    // we are evaluating for

            BoardType minMax(BoardType board); // the actual minMax function


        private:

    };
};

#endif
