
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
using ai::getMinMaxHelper;

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
    const char player_color = 'b';
    Network network = Network(0);
    auto game = ai::getCheckersGame();
    game.board.setBoardState({' ', 'r', ' ', 'r', 'r', 'r', ' ', 'r', ' ', 'r', 'r', ' ', 'r', ' ', ' ', 'r', ' ', ' ', ' ', ' ', ' ', 'b', 'b', 'b', ' ', ' ', 'b', 'b', 'b', 'b', 'b', 'b'});

    SECTION("Testing board generator"){
      cout<<"Testing minMax board generation"<<endl;
      MinMaxHelper minmax = getMinMaxHelper(player_color, network, game, 3);
      auto start = get_minmax_time();
      auto best_board = minmax.minMax();
      auto finish = get_minmax_time();
      auto run_time = finish-start;
      cout<<"those boards took "<<run_time<<" seconds "<<endl;
      REQUIRE(best_board.size());
    }

    SECTION("Testing made jumps"){
      cout<<"testing if jumps were forced"<<endl;
      MinMaxHelper minmax = getMinMaxHelper(player_color, network, game, 3);
      minmax.game.board.setBoardState({
        ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' ',
        'r',   ' ',   ' ',   ' ',
     ' ',   ' ',   'r',   ' ',
        ' ',   'b',   'b',   ' ',
     ' ',   ' ',   ' ',   ' ',
        ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' '
      });
      minmax.max_depth = 0;
      cout<<"parsing board"<<endl;
      minmax.minMax();
      cout<<"parsed"<<endl;
      vector<char> result_board_1 = {
        ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' ',
        'r',   'b',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' ',
        ' ',   'b',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' ',
        ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' '
   };
   vector<char> result_board_2 = {
     ' ',   ' ',   ' ',   ' ',
  ' ',   ' ',   ' ',   ' ',
     'r',   ' ',   'b',   ' ',
  ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   'b',   ' ',
  ' ',   ' ',   ' ',   ' ',
     ' ',   ' ',   ' ',   ' ',
  ' ',   ' ',   ' ',   ' '
      };
      bool is_one_of_these = true;
      if (minmax.game.board.getBoardState() != result_board_1){
          if (minmax.game.board.getBoardState() != result_board_2){
            is_one_of_these = false;
          }
      }
      REQUIRE(is_one_of_these);

    }

    SECTION("testing 6 turn lookahead (max_depth of 5)"){
        cout<<"Testing 6 turn lookahead"<<endl;
        MinMaxHelper minmax = getMinMaxHelper(player_color, network, game, 3);
        minmax.max_depth = 5;
        minmax.game.board.setBoardState(
          {'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', ' ', ' ', ' ', ' ', 'r', ' ', 'b', ' ', ' ', 'b', 'b', ' ', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}
        );
        auto start = get_minmax_time();
        auto best_board = minmax.minMax();
        minmax.game.board.setBoardState(best_board);
        auto finish = get_minmax_time();
        auto run_time = finish-start;
        cout<<"a lookahead of "<<minmax.max_depth + 1<<" took "<<run_time<<" seconds "<<endl;
    }
    SECTION("testing 12 turn lookahead (max_depth of 1)"){
      cout<<"Testing 12 turn lookahead"<<endl;
      MinMaxHelper minmax = getMinMaxHelper(player_color, network, game, 3);
      minmax.max_depth = 11;

      minmax.game.board.setBoardState({
             'r',  'r',  'r',  'r',
          'r',  'r',  'r',  'r',
             'r',  'r',  'r',  ' ',
          ' ',  ' ',  ' ',  'r',
             ' ',  'b',  ' ',  ' ',
          'b',  'b',  ' ',  'b',
             'b',  'b',  'b',  'b',
          'b',  'b',  'b',  'b'
      });

      auto start = get_minmax_time();
      auto best_board = minmax.minMax();
      minmax.game.board.setBoardState(best_board);
      auto finish = get_minmax_time();
      auto run_time = finish-start;
      cout<<"a lookahead of "<<minmax.max_depth + 1<<" took "<<run_time<<" seconds "<<endl;
    }

  }
