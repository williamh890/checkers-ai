#include "get-checkers-game.h"
using ai::CheckersGame;

#include "../headers/seeder.h"
using ai::getSeeder;

#include "../headers/board.h"
using ai::getBoard;

#include "../headers/player.h"
using ai::getPlayer;
using ai::getNetworkedPlayer;
using ai::Player;
using ai::RedPlayer;

#include "../headers/json-to-stl.h"

#include "../headers/move-generator.h"
using ai::getGeneratorFor;

#include <iostream>
using std::cout;
using std::endl;

CheckersGame ai::getCheckersGame() {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = getPlayer("red", converter);
  auto black = getPlayer("black", converter);
  auto board = getBoard();

  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getNetworkedCheckersGame(unsigned int red_id,
                                          unsigned int black_id) {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = getNetworkedPlayer("red", converter, red_id);
  auto black = getNetworkedPlayer("black", converter, black_id);

  auto board = getBoard();
  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getNetworkVPieceCountCheckersGame(unsigned int network_id,
                                                   char networked_player) {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = (networked_player == 'r')
                 ? getNetworkedPlayer("red", converter, network_id)
                 : getPlayer("red", converter);

  auto black = (networked_player == 'b')
                   ? getNetworkedPlayer("black", converter, network_id)
                   : getPlayer("black", converter);

  auto board = getBoard();
  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}
