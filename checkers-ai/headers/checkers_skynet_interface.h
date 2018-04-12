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
  CheckersGame game;
  std::string gameName;
  std::string serverUrl;

public:
  CheckersSkynetInterface(const std::string &serverUrl,
                          const std::string &gameName);
  CheckersSkynetInterface(const std::string &serverUrl,
                          const std::string &gameName, ai::CheckersGame &game);
  CheckersSkynetInterface(const std::string &serverUrl,
                          const std::string &gameName, unsigned int network_id,
                          const char team);
};
} // namespace ai
#endif
