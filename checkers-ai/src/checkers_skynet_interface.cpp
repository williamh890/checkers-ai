
#include "checkers_skynet_interface.h"
using ai::CheckersSkynetInterface;

#include "checkers-game.h"

CheckersSkynetInterface::CheckersSkynetInterface(
        const std::string & serverUrl,
        const std::string & gameName,
        ai::CheckersGame & game)
 : serverUrl(serverUrl), gameName(gameName) ,game(game) {}

