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
using ai::getNetworkedVPieceCount;

#include <string>
using std::string;

CheckersSkynetInterface::CheckersSkynetInterface(const string &serverUrl,
                                                 const string &gameName,
                                                 CheckersGame &game)
    : serverUrl(serverUrl), gameName(gameName), game(game) {}

CheckersSkynetInterface::CheckersSkynetInterface(const string &serverUrl,
                                                 const string &gameName)
    : serverUrl(serverUrl), gameName(gameName) {
  game = getCheckersGame();
}

CheckersSkynetInterface::CheckersSkynetInterface(const string &serverUrl,
                                                 const string &gameName,
                                                 unsigned int network_id,
                                                 const char team)
    : serverUrl(serverUrl), gameName(gameName) {
  game = getNetworkedVPieceCount(network_id, team);
}
