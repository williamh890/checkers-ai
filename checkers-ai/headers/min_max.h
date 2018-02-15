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
// ai::JsonToStlConverter

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

static const std::vector<char> COLORS = {'r', 'b'};
namespace ai{
    class MinMaxHelper{
        public:
            using BoardType = std::vector<char>;
        private:
            Network checkers_player;
            const std::string player_color;
            BoardType activeBoard;
            CheckersGame game;

            MoveGenerator redGenerator;
            MoveGenerator blackGenerator;


        public:
            MinMaxHelper() = default;
            MinMaxHelper(const std::string color, CheckersGame &game, Network network); // color is the Player
                                                                    // we are evaluating for

            BoardType minMax(BoardType board); // the actual minMax function


        private:
            int parseTree(BoardType move); // recurse on boards //return is board score best on minmax of children
            std::vector<BoardType> generateMoves(BoardType board); // returns vector of boardState
                                                                                // representing possible moves
                                                                                // recurse on these with parseTree

    };
};

#endif
