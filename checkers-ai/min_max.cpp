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

MinMaxHelper::MinMaxHelper(std::string color, CheckersGame &game, Network network):player_color(color), game(game), checkers_player(network){
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto redGenerator = getGeneratorFor("red", converter);
  auto blackGenerator = getGeneratorFor("black", converter);
  auto kingGenerator = getKingGenerator(converter);
}

int MinMaxHelper::parseTree(BoardType board){
    auto boards = generateBoards(board);
    return boards.size();
}

vector<BoardType> MinMaxHelper::generateBoards(BoardType board){
    vector<BoardType> boards;
    pair<BoardJumpsType, BoardJumpsType> boardJumps;
    pair<BoardMovesType, BoardMovesType> boardMoves;

    BoardJumpsType redJumps;
    BoardJumpsType blackJumps;

    BoardMovesType redMoves;
    BoardMovesType blackMoves;

    boardJumps = parseBoardJumps(board);
    boardMoves = parseBoardMoves(board);
    if (boardJumps.first.size()){
        redJumps = removeInvalidJumps(board, boardJumps.first);
    }
    if (boardJumps.second.size()){
        blackJumps = removeInvalidJumps(board, boardJumps.second);
    }

    if(not redJumps.size()){
        redMoves = removeInvalidMoves(board, boardMoves.first);
        }
    if (not blackJumps.size()){
        blackMoves = removeInvalidMoves(board, boardMoves.second);
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

BoardType MinMaxHelper::minMax(BoardType board)
{
    return board;
}

pair<BoardJumpsType, BoardJumpsType> MinMaxHelper::parseBoardJumps(BoardType board){
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


pair<BoardMovesType, BoardMovesType> MinMaxHelper::parseBoardMoves(BoardType board){
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

BoardJumpsType MinMaxHelper::removeInvalidJumps(BoardType board, BoardJumpsType jumps){
    BoardJumpsType validJumps{};

    char color = board[jumps[0].first];
    color = tolower(int(color));
    char enemy_color;
    for(auto i = 0; i < COLORS.size(); i++){
        if (color != COLORS[i]){
          enemy_color = COLORS[i];
          break;
        }
    }

    for(auto j = 0; j < jumps.size(); j++){
        for(auto i = 0; i < jumps[j].second.size(); i++){
          if(not(board[jumps[j].second[i].to] != ' ' or tolower(board[(jumps[j].second[i].through)])  != enemy_color)){
              validJumps.push_back(jumps[j]);
              }
           }
        }
      return validJumps;
    }

BoardMovesType MinMaxHelper::removeInvalidMoves(BoardType board, BoardMovesType moves){
    BoardMovesType validMoves{};

    char color = board[moves[0].first];
    color = tolower(int(color));
    char enemy_color;
    for(auto i = 0; i < COLORS.size(); i++){
        if (color != COLORS[i]){
          enemy_color = COLORS[i];
          break;
          }
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

vector<BoardType> MinMaxHelper::_generate_boards(BoardType board, BoardJumpsType jumps){
    vector<BoardType> genBoards{};

    for(int i = 0; i < jumps.size(); i++){
        for (auto j = 0; j < jumps[i].second.size(); j++){
            auto new_board = board;
            new_board[jumps[i].second[j].to] = new_board[jumps[i].first];
            new_board[jumps[i].first] = ' ';
            new_board[jumps[i].second[j].through] = ' ';
            genBoards.push_back(new_board);
          }
    }
  return genBoards;
}

vector<BoardType> MinMaxHelper::_generate_boards(BoardType board, BoardMovesType moves){
    vector<BoardType> genBoards{};
    for(int i = 0; i < moves.size(); i++){
        for (auto j = 0; j < moves[i].second.size(); j++){
            auto new_board = board;
            new_board[moves[i].second[j]] = new_board[moves[i].first];
            new_board[moves[i].first] = ' ';
          }
        }
    return genBoards;
      }
