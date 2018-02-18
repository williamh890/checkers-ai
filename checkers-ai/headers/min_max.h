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

#include "json-to-stl.h"
// ai::JsonToStlConverter

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

static const std::vector<char> COLORS = {'r', 'b'};
static const char RED = 'r';
static const char BLACK = 'b';
namespace ai{
    class MinMaxHelper{
        public:
            using BoardType = std::vector<char>;
            using MovesType = std::vector<int>;
            using JumpsType = std::vector<Jump>;
            using BoardMovesType = std::vector<std::pair<int, MovesType>>;
            using BoardJumpsType = std::vector<std::pair<int, JumpsType>>;
            CheckersGame game;
        private:
            Network checkers_player;
            std::string player_color;
            BoardType activeBoard;

            MoveGenerator redGenerator;
            MoveGenerator blackGenerator;
            MoveGenerator kingGenerator;


        public:
            std::vector<BoardType> parseTree();
            std::vector<BoardType> parseTree(const BoardType board, int depth, const int & max_depth);
            MinMaxHelper() = default;
            MinMaxHelper(const MoveGenerator & redGenerator, const MoveGenerator & blackGenerator, const MoveGenerator & kingGenerator, const std::string color, Network network, CheckersGame & game); // color is the Player
                                                                    // we are evaluating for

            int minMax(); // the actual minMax function


        private:
             // recurse on boards //return is board score best on minmax of children
            std::vector<BoardType> generateBoards(BoardType board); // returns vector of boardState
            std::pair<BoardMovesType, BoardMovesType> parseBoardMoves(BoardType board);
            std::pair<BoardJumpsType, BoardJumpsType> parseBoardJumps(BoardType board);

            BoardJumpsType removeInvalidJumps(BoardType & board,  BoardJumpsType & jumps);
            BoardMovesType removeInvalidMoves(BoardType & board, BoardMovesType & moves);
            std::vector<BoardType> _generate_boards(BoardType & board, BoardMovesType & moves);
            std::vector<BoardType> _generate_boards(BoardType & board, BoardJumpsType & jumps);

          };

    MinMaxHelper getMinMaxHelper(const std::string color, Network network, CheckersGame & game);
}

#endif
