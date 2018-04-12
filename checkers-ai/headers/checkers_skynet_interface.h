#ifndef CHECKERS_SKYNET_INTERFACE_H
#define CHECKERS_SKYNET_INTERFACE_H

#include "checkers-game.h"
// ai::CheckersGame
#include "json.hpp"

#include "../../skynet/include/jsoncpp/json.h"
#include "../../skynet/src/skynet/checkers.hpp"
#include "../../skynet/src/skynet/checkers_client.hpp"
#include <string>
// std::string

namespace ai {
class CheckersSkynetInterface {
private:
  std::string serverUrl;
  std::string gameName;

private:
  CheckersGame game;
  std::string GameName;
  std::string serverUrl;

public:
  CheckersSkynetInterface(const std::string &serverName,
                          const std::string &gameName, ai::CheckersGame &game);

  CheckersSkynetInterface(CheckersGame &game, std::string game_name);

} // namespace ai
#endif
