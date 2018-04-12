#include "../../skynet/src/skynet/checkers.hpp"
#include "../../skynet/src/skynet/checkers_client.hpp"
using skynet::checkers::game_info;
using skynet::checkers::play_game;
using skynet::checkers::print_game;
#include "checkers_skynet_interface.h"
using ai::CheckersSkynetInterface;
#include "checkers-game.h"
using ai::CheckersGame;
#include "json.hpp"

#include <string>
using std::string;

CheckersSkynetInterface::CheckersSkynetInterface(CheckersGame &game,
                                                 string game_name)
    : game(game), game_name(game_name) {}
