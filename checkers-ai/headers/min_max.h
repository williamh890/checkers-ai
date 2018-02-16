// min_max.h

#ifndef MIN_MAX_H
#define MIN_MAX_H
#include "checkers-game.h"
// ai::CheckersGame

#include <utility>
// std::make_pair
// std::pair

#include <vector>
// std::vector

static const std::vector<char> COLORS = {'r', 'b'};
namespace ai {
    int minimax(std::pair<int, int> move, int depth, char maximizingPlayer, CheckersGame game);
}
#endif
