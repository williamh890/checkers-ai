
#include "checkers_skynet_interface.h"
using ai::SkynetCheckersGame;

#include "checkers-game.h"

SkynetCheckersGame::SkynetCheckersGame(
        const std::string & serverUrl,
        const std::string & gameName,
        ai::CheckersGame & game)
 : serverUrl(serverUrl), gameName(gameName) ,game(game) {}

