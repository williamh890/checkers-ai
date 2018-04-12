#include "../../skynet/src/skynet/checkers.hpp"
#include "../../skynet/src/skynet/checkers_client.hpp"
using skynet::checkers::info_game;
using skynet::checkers::play_game;
#include "checkers_skynet_interface.h"
using ai::CheckersSkynetInterface;

#include "checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include <string>
using std::string;

CheckersSkynetInterface::CheckersSkynetInterface(const string &serverUrl,
                                                 const string &gameName,
                                                 CheckersGame &game)
    : serverUrl(serverUrl), gameName(gameName), game(game) {}

