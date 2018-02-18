#include "../headers/min_max.h"
using ai::MinMaxHelper;

#include "../headers/move-generator.h"
using ai::getGeneratorFor;
using ai::getKingGenerator;
using ai::MoveGenerator;

#include "../headers/json-to-stl.h"
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

#include <iostream>
using std::cout;
using std::endl;

#include "catch.hpp"

#include <chrono>

double get_minmax_time() {
    return 1.0e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count();
}

TEST_CASE("testing min max class"){
    const string player_color = "b";
    Network network = Network(0);
    auto game = getCheckersGame();

    SECTION("Testing board generator"){
      cout<<"Testing minMax board generation"<<endl;
      MinMaxHelper minmax = getMinMaxHelper(player_color, network, game);
      auto start = get_minmax_time();
      auto count = minmax.minMax();
      auto finish = get_minmax_time();
      auto run_time = finish-start;
      cout<<count<<" boards took "<<run_time<<" seconds "<<endl;
      cout<<"that's "<< count/run_time <<" boards per second"<<endl;
      cout<<"or "<<run_time/count<<" seconds per board on average"<<endl;
      REQUIRE(count);

  }
}
