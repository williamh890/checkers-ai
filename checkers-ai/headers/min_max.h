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
static const int KING_WEIGHT = 2;
static const int PAWN_WEIGHT = 1;

namespace ai{
    class MinMaxHelper{
        public:
            using PlayableMoveType = std::pair<int, int>;
            using PlayableJumpType = std::pair<int, Jump>;
            using PlayableMovesType = std::vector<PlayableMoveType>;
            using PlayableJumpsType = std::vector<PlayableJumpType>;
            using BoardType = std::vector<char>;
            using MovesType = std::vector<int>;
            using JumpsType = std::vector<Jump>;
            using BoardMovesType = std::vector<std::pair<int, MovesType>>;
            using BoardJumpsType = std::vector<std::pair<int, JumpsType>>;
            int max_depth;
            CheckersGame game;
        private:
            Network checkers_player;
            const char player_color;
            const char enemy_color;
            BoardType activeBoard;

            MoveGenerator redGenerator;
            MoveGenerator blackGenerator;
            MoveGenerator kingGenerator;
            MoveGenerator activeGenerator;

            bool jumped = false;
            int jumped_to = 0;
            char active_player;
            char inactive_player;

        public:
            long int boards_done = 0;
            long int legal_moves_looked_at = 0;
            long int legal_jumps_looked_at = 0;

            PlayableMovesType nextMoves;
            PlayableJumpsType nextJumps;

            PlayableMoveType nextMove;
            PlayableJumpType nextJump;

            BoardType parseTree();
            int parseTree(const BoardType board, int depth);
            MinMaxHelper() = default;
            MinMaxHelper(const MoveGenerator & redGenerator, const MoveGenerator & blackGenerator, const MoveGenerator & kingGenerator, const char color, const char opponent_color, Network network, CheckersGame & game, int max_depth); // color is the Player
                                                                    // we are evaluating for

            BoardType minMax(); // the actual minMax function

            void makeJump();
            void makeMove();

        private:
             // recurse on boards //return is board score best on minmax of children
            std::vector<BoardType> generateBoards(BoardType board); // returns vector of boardState
            std::vector<BoardType> generateBoardsSetMoves(BoardType board);

            BoardMovesType parseBoardMoves(BoardType board);
            BoardJumpsType parseBoardJumps(BoardType board);

            BoardJumpsType removeInvalidJumps(BoardType & board,  BoardJumpsType & jumps);
            BoardMovesType removeInvalidMoves(BoardType & board, BoardMovesType & moves);
            std::vector<BoardType> _generate_boards(BoardType & board, BoardMovesType & moves);
            std::vector<BoardType> _generate_boards(BoardType & board, BoardJumpsType & jumps);
            int evaluateBoard(BoardType & board);
            void swapActivePlayer();
            void setNextJumps(BoardJumpsType & jumps);
            void setNextMoves(BoardMovesType & moves);


          };

    MinMaxHelper getMinMaxHelper(const char & color, Network network, CheckersGame & game, int max_depth);
}

#endif
