
#include "checkers-game.h"

#include "search.h"
using ai::SearchHelper;

#include <string>

int main(int argc, char **argv) {

  unsigned int red_id = std::stoi(argv[1]);
  unsigned int black_id = std::stoi(argv[2]);
  SearchHelper::limit = std::stof(argv[3]);

  auto game = getNetworkedCheckersGame(red_id, black_id);
}
