#ifndef CHECKERS_SKYNET_INTERFACE_H
#define CHECKERS_SKYNET_INTERFACE_H
#include "../../skynet/include/jsoncpp/json.h"
#include "../../skynet/src/json/json.hpp"
#include "../../skynet/src/skynet/checkers.hpp"
#include "../../skynet/src/skynet/checkers_client.hpp"
// skynet::checkers::play_game
// skynet::checkers::print_game
// skynet::checkers::game_info
// skynet::checkers::game_info_t
#include "checkers-game.h"
// ai::CheckersGame
#include "json.hpp"

#include <string>
// std::string

namespace ai {
class CheckersSkynetInterface {
public:
  using game_info_type = skynet::checkers::game_info_t;

private:
  CheckersGame game;
  std::string game_name;

public:
  CheckersSkynetInterface() = default;
  ~CheckersSkynetInterface() = default;

  CheckersSkynetInterface(CheckersGame &game, std::string game_name);

  void sendBoard();
  void receiveBoard();

private:
};
} // namespace ai
#endif
