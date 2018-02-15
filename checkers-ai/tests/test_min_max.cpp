#include "../headers/min_max.h"
using ai::MinMaxHelper;

#include "../headers/move-generator.h"
using ai::getGeneratorFor;
using ai::getKingGenerator;
using ai::MoveGenerator;
using ai::JsonToStlConverter;
using ai::loadMoveTableFrom;

#include "../headers/models.h"
using ai::Jump;

#include "../headers/consts.h"
using ai::TOTAL_NUM_SPACES;

#include "../headers/checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;

#include "../headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include "../headers/network.h"
using ai::Network;

#include<utility>
using std::pair;
using std::make_pair;

#include <vector>
using std::vector;

#include <string>
using std::string;


#include "catch.hpp"

TEST_CASE("testing min max class"){
    string player_color = "r";
    CheckersGame game = getCheckersGame();
    Network network = Network(0);

    SECTION("Testing Constructor of MinMaxHelper"){
        MinMaxHelper minmax = MinMaxHelper(player_color, game, network);
      }
}
