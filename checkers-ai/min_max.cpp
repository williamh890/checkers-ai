// min_max.cpp
#include "headers/consts.h"
using ai::TOTAL_NUM_SPACES;
#include "headers/min_max.h"
using ai::MinMaxHelper;
using MovesType = MinMaxHelper::MovesType;
using JumpsType = MinMaxHelper::JumpsType;
using BoardType = MinMaxHelper::BoardType;
using BoardMovesType = MinMaxHelper::BoardMovesType;
using BoardJumpsType = MinMaxHelper::BoardJumpsType;


#include "headers/network.h"
using ai::Network;

#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include "headers/move-generator.h"
using ai::MoveGenerator;
using ai::getGeneratorFor;
using ai::getKingGenerator;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;
using ai::loadMoveTableFrom;

#include <cctype>
using std::tolower;
using std::isupper;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <algorithm>
using std::max;

#include <utility>
using std::make_pair;
using std::pair;
using std::swap;

#include <iostream>
using std::cout;
using std::endl;


MinMaxHelper ai::getMinMaxHelper(const char & color, Network network, CheckersGame & game, int max_depth){
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};
    auto red = getGeneratorFor("red", converter);
    auto black = getGeneratorFor("black", converter);
    auto king = getKingGenerator(converter);
    char opponent;
    if (color == 'r'){
      opponent = 'b';
    }
    else{
      opponent = 'r';
    }
    const char enemy = opponent;
    return MinMaxHelper(red, black, king, color, enemy, network, game, max_depth);
  }

MinMaxHelper::MinMaxHelper(const MoveGenerator & redGenerator, const MoveGenerator & blackGenerator, const MoveGenerator & kingGenerator, const char color, const char opponent_color, Network network, CheckersGame & game, int max_depth):redGenerator(redGenerator), blackGenerator(blackGenerator), kingGenerator(kingGenerator), player_color(color), enemy_color(opponent_color), checkers_player(network), game(game), max_depth(max_depth){
}

int MinMaxHelper::parseTree(const BoardType board, int depth){
    auto layer_jump = jumped;
    auto layer_player = active_player;
    auto boards = generateBoards(board);
    int count = 0;
    int best_count = 0;

    if (depth == max_depth){
        for (int i = boards.size()-1; i>= 0; i--){
            count = evaluateBoard(boards[i]);
            best_count = max(best_count, count);
        }
        return best_count;
    }

    depth += 1;

    for (int i = boards.size()-1; i >= 0; i--){
        best_count = max(best_count, parseTree(boards[i], depth));
        jumped = layer_jump;
        active_player = layer_player;
      }
    return best_count;
  }

BoardType MinMaxHelper::parseTree(){
    jumped = false;
    active_player = player_color;
    inactive_player = enemy_color;
    swapActivePlayer(); // i set up the move generator also, do not remove this
    auto layer_player = active_player;
    auto layer_jump = jumped;

    int depth = 0;
    int best_count = 0;
    int count = 0;
    BoardType best_board;

    BoardType current_board = game.getBoard();

    auto boards = generateBoards(current_board);
    if (depth == max_depth){
        for (int i = boards.size()-1; i>= 0; i--){
            count = evaluateBoard(boards[i]);
            best_count = max(best_count, count);
            if (best_count == count){
                best_board = boards[i];
              }
          }
        cout<<"best count was "<< best_count<<endl;
        return best_board;
    }

    depth += 1;

    for (int i = boards.size()-1; i >= 0; i--){
        count = parseTree(boards[i], depth);
        active_player = layer_player;
        jumped = layer_jump;
        best_count = max(best_count, count);
        if (best_count == count){
            best_board = boards[i];
        }
      }
    cout<<"best count was "<<best_count<<endl;
    return best_board;
}

vector<BoardType> MinMaxHelper::generateBoards(BoardType board){
    if (not jumped){
        swapActivePlayer();
    }
    vector<BoardType> boards;
    BoardJumpsType boardJumps;
    BoardMovesType boardMoves;

    boardJumps = parseBoardJumps(board); // gets all possible jumps from each occupied spot
    auto Jumps = removeInvalidJumps(board, boardJumps);
    if (jumped){
        if (Jumps.size() <= 0){
            swapActivePlayer();
            boardJumps = parseBoardJumps(board);
            Jumps = removeInvalidJumps(board, boardJumps);
        }
    }
    boardMoves = parseBoardMoves(board); // above, but for moves


    if (boardJumps.size()){
        if(not Jumps.size()){
            auto Moves = removeInvalidMoves(board, boardMoves);
            jumped = false;
            return _generate_boards(board, Moves);
        }
        jumped = true;
        return _generate_boards(board, Jumps);
    }
  }

BoardType MinMaxHelper::minMax(){
    BoardType board = parseTree();
    game.board.setBoardState(board);
    return board;
}
 BoardJumpsType MinMaxHelper::parseBoardJumps(BoardType board){
    BoardJumpsType Jumps{};
    for (auto i = 0; i < TOTAL_NUM_SPACES; i++){
        if (isupper(board[i])){
          if (tolower(board[i]) == active_player){
            Jumps.push_back(make_pair(i, kingGenerator.getJumps(i)));
              }
            }
        else if (board[i] == active_player){
          Jumps.push_back(make_pair(i, activeGenerator.getJumps(i)));
            }
        }
    return Jumps;
  }


  BoardMovesType MinMaxHelper::parseBoardMoves(BoardType board){
    BoardMovesType Moves{};
    for (auto i = 0; i < TOTAL_NUM_SPACES; i++){
        if (isupper(board[i])){
          if (tolower(board[i]) == active_player){
            Moves.push_back(make_pair(i, kingGenerator.getMoves(i)));
              }
            }
        else if (board[i] == active_player){
          Moves.push_back(make_pair(i, activeGenerator.getMoves(i)));
            }
        }
    return Moves;
  }

BoardJumpsType MinMaxHelper::removeInvalidJumps(BoardType & board, BoardJumpsType & jumps){
    BoardJumpsType validJumps{};

    for(auto j = 0; j < jumps.size(); j++){
        for(auto i = 0; i < jumps[j].second.size(); i++){
          if(board[jumps[j].second[i].to] == ' ' and tolower(board[(jumps[j].second[i].through)])  == inactive_player){
              validJumps.push_back(jumps[j]);
              }
           }
        }
      return validJumps;
    }
BoardMovesType MinMaxHelper::removeInvalidMoves(BoardType & board, BoardMovesType & moves){
    BoardMovesType validMoves{};
    for(auto j = 0; j < moves.size(); j++){
        for(auto i = 0; i < moves[j].second.size(); i++){
            if(board[moves[j].second[i]] == ' '){
                validMoves.push_back(moves[j]);
            }
        }
    }
    return validMoves;
}
vector<BoardType> MinMaxHelper::_generate_boards(BoardType & board, BoardJumpsType & jumps){
    vector<BoardType> genBoards{};
    genBoards.reserve(jumps.size());
    for(int i = 0; i < jumps.size(); i++){
        for (auto j = 0; j < jumps[i].second.size(); j++){
            auto new_board = board;
            swap(new_board[jumps[i].second[j].to], new_board[jumps[i].first]);
            new_board[jumps[i].second[j].through] = ' ';
            genBoards.push_back(new_board);
          }
    }
  return genBoards;
}
vector<BoardType> MinMaxHelper::_generate_boards(BoardType & board, BoardMovesType & moves){
    vector<BoardType> genBoards{};
    genBoards.reserve(moves.size());
    for(int i = 0; i < moves.size(); i++){
        for (auto j = 0; j < moves[i].second.size(); j++){
            auto new_board = board;
            swap(new_board[moves[i].second[j]], new_board[moves[i].first]);
            genBoards.push_back(new_board);
          }
        }
    return genBoards;
      }

int MinMaxHelper::evaluateBoard(BoardType & board){
    int count = 0;
    for (auto i = 0; i < board.size(); i++){
        if (board[i] == ' '){
          continue;
        }
        if (isupper(board[i])){
          if (tolower(board[i]) == active_player){
              count += 2;
          }
          else{
              count -= 2;
          }
        }
        else{
          if ((board[i]) == active_player){
            count += 1;
          }
          else{
              count -= 1;
            }
          }
        }
    return count;
  }

  void MinMaxHelper::swapActivePlayer(){
      if (active_player == RED){
          activeGenerator = blackGenerator;
      }
      else{
          activeGenerator = redGenerator;
      }
      swap(active_player, inactive_player);
  }
