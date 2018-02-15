// min_max.cpp

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
using ai::JsonToStlConverter;

#include <cctype>
using std::tolower;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <utility>
using std::make_pair;
using std::pair;

MinMaxHelper::MinMaxHelper(std::string color, CheckersGame &game, Network network):player_color(color), game(game), checkers_player(network){
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto redGenerator = getGeneratorFor("red", converter);
  auto blackGenerator = getGeneratorFor("black", converter);
  auto kingGenerator = getKingGenerator(converter);
}

int MinMaxHelper::parseTree(BoardType board){
    int total = 0;
    return total;
}

vector<BoardType> MinMaxHelper::generateBoards(BoardType board){
    pair<BoardJumpsType, BoardJumpsType> boardJumps;
    pair<BoardMovesType, BoardMovesType> boardMoves;

    boardJumps = parseBoardJumps(board);
    boardMoves = parseBoardMoves(board);
    if (boardJumps.first.size() && boardJumps.second.size()){

    }
    BoardType help = {'h', 'e', 'l', 'p'};
    vector<BoardType> moves = {help};
    return moves;
}

BoardType MinMaxHelper::minMax(BoardType board)
{
    return board;
}

pair<BoardJumpsType, BoardJumpsType> MinMaxHelper::parseBoardJumps(BoardType &board){
    BoardJumpsType redJumps{};
    BoardJumpsType blackJumps{};

    for (auto i = 0; i < board.size(); i++){
        if (board[i] == ' '){
          continue;
        }
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

pair<BoardMovesType, BoardMovesType> MinMaxHelper::parseBoardMoves(BoardType &board){
    BoardMovesType redMoves{};
    BoardMovesType blackMoves{};
    for (auto i = 0; i < board.size(); i++){
        if (board[i] == ' '){
          continue;
        }
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

BoardJumpsType removeInvalidJumps(BoardType board, BoardJumpsType jumps){
    char color = board[jumps[0].first];
    color = tolower(int(color));
    char enemy_color;
    for(auto i = 0; i < COLORS.size(); i++){
        if (color != COLORS[i]){
          char enemy_color = COLORS[i];
          break;
        }
    }

    BoardJumpsType validJumps{};

    for(auto j = 0; j < jumps.size(); j++){
        for(auto i = 0; i < jumps[j].second.size(); i++){
          if(not(board[jumps[j].second[i].to] != ' ' or tolower(board[(jumps[j].second[i].through)])  != enemy_color)){
              validJumps.push_back(jumps[j]);
              }
           }
        }
      return validJumps;
    }
