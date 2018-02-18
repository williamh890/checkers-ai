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

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <utility>
using std::make_pair;
using std::pair;
using std::swap;

#include <iostream>
using std::cout;
using std::endl;


MinMaxHelper ai::getMinMaxHelper(const std::string color, Network network, CheckersGame & game){
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};
  auto red = getGeneratorFor("red", converter);
  auto black = getGeneratorFor("black", converter);
  auto king = getKingGenerator(converter);
  return MinMaxHelper(red, black, king, color, network, game);
}

MinMaxHelper::MinMaxHelper(const MoveGenerator & redGenerator, const MoveGenerator & blackGenerator, const MoveGenerator & kingGenerator, const string color, Network network, CheckersGame & game):redGenerator(redGenerator), blackGenerator(blackGenerator), kingGenerator(kingGenerator), player_color(color), checkers_player(network), game(game){
}

vector<BoardType> MinMaxHelper::parseTree(const BoardType board, int depth, const int & max_depth){
  auto boards = generateBoards(board);
  if (depth == max_depth){
      return boards;
  }
  depth += 1;
  for (int i = boards.size()-1; i >= 0; i--){
      auto new_boards = parseTree(boards[i], depth, max_depth);
      boards.reserve(boards.size() + distance(new_boards.begin(), new_boards.end()));
      boards.insert(boards.end(), new_boards.begin(), new_boards.end());
    }
  return boards;
}

vector<BoardType> MinMaxHelper::parseTree(){
    int depth = 0;
    const int max_depth = 3;
    BoardType current_board = game.getBoard();
    auto boards = generateBoards(current_board);
    for (int i = boards.size()-1; i >= 0; i--){
        auto new_boards = parseTree(boards[i], depth, max_depth);
        boards.reserve(boards.size() + distance(new_boards.begin(), new_boards.end()));
        boards.insert(boards.end(), new_boards.begin(), new_boards.end());
      }
    return boards;
}

vector<BoardType> MinMaxHelper::generateBoards(BoardType board){
    vector<BoardType> boards;
    pair<BoardJumpsType, BoardJumpsType> boardJumps;
    pair<BoardMovesType, BoardMovesType> boardMoves;

    BoardJumpsType redJumps;
    BoardJumpsType blackJumps;

    BoardMovesType redMoves;
    BoardMovesType blackMoves;

    boardJumps = parseBoardJumps(board); // gets all possible jumps from each occupied spot
    boardMoves = parseBoardMoves(board); // above, but for moves


    if (boardJumps.first.size()){
        redJumps = removeInvalidJumps(board, boardJumps.first);
        if(not redJumps.size()){
            redMoves = removeInvalidMoves(board, boardMoves.first);
        }
    }
    if (boardJumps.second.size()){
        blackJumps = removeInvalidJumps(board, boardJumps.second);
        if (not blackJumps.size()){
            blackMoves = removeInvalidMoves(board, boardMoves.second);
        }
    }

    if (player_color == "r"){
        if(redJumps.size()){
            return _generate_boards(board, redJumps);
        }
        else{
          return _generate_boards(board, redMoves);
        }
    }
    else{
        if(blackJumps.size()){
            return _generate_boards(board, blackJumps);
          }
        else{
            return _generate_boards(board, blackMoves);
          }
        }
    }
int MinMaxHelper::minMax()
{
    auto boards = parseTree();
    return boards.size();
}
pair<BoardJumpsType, BoardJumpsType> MinMaxHelper::parseBoardJumps(BoardType board){
    BoardJumpsType redJumps{};
    BoardJumpsType blackJumps{};
    for (auto i = 0; i < TOTAL_NUM_SPACES; i++){
        if (board[i] == 'r'){
            redJumps.push_back(make_pair(i, redGenerator.getJumps(i)));

        }
        else if (board[i] == 'b'){
            blackJumps.push_back(make_pair(i, blackGenerator.getJumps(i)));

        }
        else if (board[i] == 'R'){
          redJumps.push_back(make_pair(i, kingGenerator.getJumps(i)));
        }
        else if (board[i] == 'B'){
          blackJumps.push_back(make_pair(i, kingGenerator.getJumps(i)));
        }
    }
    return make_pair(redJumps, blackJumps);
  }


pair<BoardMovesType, BoardMovesType> MinMaxHelper::parseBoardMoves(BoardType board){
    BoardMovesType redMoves{};
    BoardMovesType blackMoves{};
    for (auto i = 0; i < board.size(); i++){
        if (board[i] == 'r'){
            redMoves.push_back(make_pair(i, redGenerator.getMoves(i)));

        }
        else if (board[i] == 'b'){
            blackMoves.push_back(make_pair(i, blackGenerator.getMoves(i)));
          }

        else if (board[i] == 'R'){
          redMoves.push_back(make_pair(i, kingGenerator.getMoves(i)));
        }
        else if (board[i] == 'B'){
          blackMoves.push_back(make_pair(i, kingGenerator.getMoves(i)));
        }
    }
    return make_pair(redMoves, blackMoves);
}

BoardJumpsType MinMaxHelper::removeInvalidJumps(BoardType & board, BoardJumpsType & jumps){
    BoardJumpsType validJumps{};
    char enemy_color = RED;
    if (tolower(board[jumps[0].first]) == RED){
        enemy_color = BLACK;
      }

    for(auto j = 0; j < jumps.size(); j++){
        for(auto i = 0; i < jumps[j].second.size(); i++){
          if(board[jumps[j].second[i].to] == ' ' and tolower(board[(jumps[j].second[i].through)])  == enemy_color){
              validJumps.push_back(jumps[j]);
              }
           }
        }
      return validJumps;
    }
BoardMovesType MinMaxHelper::removeInvalidMoves(BoardType & board, BoardMovesType & moves){
    BoardMovesType validMoves{};
    char enemy_color = RED;
    if (tolower(board[moves[0].first]) == RED){
        enemy_color = BLACK;
      }

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
