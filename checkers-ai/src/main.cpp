#include "checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "network.h"
using ai::Network;
using ai::weightChangeOut;

#include "utils.h"
using ai::getTime;

#include "search.h"
using ai::SearchHelper;

#include "consts.h"
using ai::MOVE_LIMIT;

#include "checkers_skynet_interface.h"
using ai::CheckersSkynetInterface;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::stof;
using std::stoi;
using std::string;

int main(int argc, char **argv) {

  auto start = getTime();

  unsigned int red_id = stoi(argv[1]);
  unsigned int black_id = stoi(argv[2]);
  SearchHelper::limit = stof(argv[3]);

#ifndef USE_IDS
  CheckersGame::SEARCH_DEPTH = stoi(argv[3]);
#endif


  auto game = getNetworkedCheckersGame(red_id, black_id);
  auto interface = CheckersSkynetInterface("localhost:8080", "test", game);

  const char winner = game.play();

  auto end = getTime();

  cout << "game: " << end - start << endl;
  if (game.moveCounter >= MOVE_LIMIT) {
    return 0;
  }

  if (winner == 'r') {
    return 1;
  } else {
    return 255;
  }
}
