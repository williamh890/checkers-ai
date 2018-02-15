// min_max.cpp

#include "headers/min_max.h"
using ai::MinMaxHelper;
using BoardType = MinMaxHelper::BoardType;

#include "headers/network.h"
using ai::Network;

#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include "headers/move-generator.h"
using ai::MoveGenerator;

#include <vector>
using std::vector;

using std::string;

MinMaxHelper::MinMaxHelper(const std::string color, CheckersGame &game, Network network):player_color(color), game(game), checkers_player(network){
}

int MinMaxHelper::parseTree(BoardType board){

  return 0; //dummy
}
