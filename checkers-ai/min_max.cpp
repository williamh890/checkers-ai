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
using std::min;

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

    const char enemy = (color == 'r') ?
                        BLACK: RED;

    return MinMaxHelper(red, black, king, color, enemy, network, game, max_depth);
  }

MinMaxHelper::MinMaxHelper(const MoveGenerator & redGenerator, const MoveGenerator & blackGenerator, const MoveGenerator & kingGenerator, const char color, const char opponent_color, Network network, CheckersGame & game, int max_depth):redGenerator(redGenerator), blackGenerator(blackGenerator), kingGenerator(kingGenerator), player_color(color), enemy_color(opponent_color), checkers_player(network), game(game), max_depth(max_depth){
}

BoardType MinMaxHelper::parseTree(){
    jumped = false;
    active_player = player_color;
    inactive_player = enemy_color;
    auto layer_player = active_player;
    auto layer_jump = jumped;
    swapActivePlayer();

    int depth = 0;
    int count = 0;
    int best_count = 0;


    BoardType best_board;
    BoardType current_board = game.board.getBoardState();

    auto boards = generateBoardsSetMoves(current_board);
    boards_done += boards.size();

    if (depth == max_depth){
        for (int i = boards.size()-1; --i;){
            count = evaluateBoard(boards[i]);
            best_count = max(best_count, count);
            if (best_count == count){
                best_board = boards[i];
                if (jumped) { nextJump = nextJumps[i]; }
                else { nextMove = nextMoves[i]; }
              }
          }
        cout<<"best count was "<< best_count<<endl;
        return best_board;
    }

    depth += 1;

    int best_index;
    cout<<"we have "<<boards.size()<<" boards to look at "<<endl;
    for (int i = boards.size()-1; i>=0; --i){
        cout<<" looking at board "<<i<<endl;
        count = parseTree(boards[i], depth);
        active_player = layer_player;
        jumped = layer_jump;
        best_count = max(best_count, count);
        if (best_count == count){
            best_board = boards[i];
            best_index = i;
            game.board.setBoardState(best_board);
            cout<<" a better board was "<<endl<<game.board.toString()<<endl;
          }
        }

    if (jumped) { nextJump = nextJumps[best_index];}
    else { nextMove = nextMoves[best_index]; }

    cout<<"best count was "<<best_count<<endl;
    return best_board;
}

int MinMaxHelper::parseTree(const BoardType board, int depth){
    auto boards = generateBoards(board);
    auto layer_jump = jumped;
    auto layer_player = active_player;

    boards_done += boards.size();

    int count = 0;
    int best_count = 0;

    if (depth == max_depth){
        for (int i = boards.size()-1; i>= 0; --i){
            count = evaluateBoard(boards[i]);
            best_count = max(best_count, count);
        }
        return best_count;
    }

    depth += 1;

    for (int i = boards.size()-1; i >= 0; --i){
        int best_count = (layer_player == player_color) ?
             max(best_count, parseTree(boards[i], depth)):
             min(best_count, parseTree(boards[i], depth));
        jumped = layer_jump;
        active_player = layer_player;;
      }
    return best_count;
  }

vector<BoardType> MinMaxHelper::generateBoardsSetMoves(BoardType board){
    swapActivePlayer();
    auto boardJumps = parseBoardJumps(board);
    if (boardJumps.size()){
      auto Jumps = removeInvalidJumps(board, boardJumps);
      if (Jumps.size()){
        setNextJumps(Jumps);
        swapActivePlayer();
        return generateBoards(board);
      }
    }
    auto boardMoves = parseBoardMoves(board);
    auto Moves = removeInvalidMoves(board, boardMoves);
    setNextMoves(Moves);
    swapActivePlayer();
    return generateBoards(board);
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
    legal_jumps_looked_at += Jumps.size();
    if (jumped){
        if (not Jumps.size()){
            swapActivePlayer();
            boardJumps = parseBoardJumps(board);
            Jumps = removeInvalidJumps(board, boardJumps);
            legal_jumps_looked_at += Jumps.size();
        }
    }

    boardMoves = parseBoardMoves(board); // above, but for moves

    if(not Jumps.size()){
        auto Moves = removeInvalidMoves(board, boardMoves);
        legal_moves_looked_at += Moves.size();
        jumped = false;
        return _generate_boards(board, Moves);
        }
    jumped = true;
    return _generate_boards(board, Jumps);
  }

BoardType MinMaxHelper::minMax(){
    cout<<game.board.toString()<<endl;
    BoardType board = parseTree();
    game.board.setBoardState(board);
    cout<<game.board.toString()<<endl;
    cout<<" did "<<boards_done<< " boards in this run "<<endl;
    cout<<" looked at "<<legal_moves_looked_at<<" legal moves "<<endl;
    cout<<" looked at "<<legal_jumps_looked_at<<" legal jumps "<<endl;
    return board;
}
 BoardJumpsType MinMaxHelper::parseBoardJumps(BoardType board){
    BoardJumpsType Jumps{};
    for (size_t i = 0; i < TOTAL_NUM_SPACES; i++){
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
    for (size_t i = 0; i < TOTAL_NUM_SPACES; i++){
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

    for (size_t j = 0; j < jumps.size(); ++j){
        for (size_t i = 0; i < jumps[j].second.size(); ++i){
          if(board[jumps[j].second[i].to] == ' ' and tolower(board[(jumps[j].second[i].through)])  == inactive_player){
              validJumps.push_back(jumps[j]);
              }
           }
        }
    return validJumps;
  }

BoardMovesType MinMaxHelper::removeInvalidMoves(BoardType & board, BoardMovesType & moves){
    BoardMovesType validMoves{};
    for (size_t j = 0; j < moves.size(); ++j){
        for (size_t i = 0; i < moves[j].second.size(); ++i){
            if(board[moves[j].second[i]] == ' '){
                validMoves.push_back(moves[j]);
            }
        }
    }

    return validMoves;
}

vector<BoardType> MinMaxHelper::_generate_boards(BoardType & board, BoardJumpsType & jumps){
    vector<BoardType> genBoards;

    genBoards.reserve(jumps.size());
    BoardType new_board;

    for (size_t i = 0; i < jumps.size(); ++i){
        for (size_t j = 0; j < jumps[i].second.size(); ++j){
            new_board = board;
            swap(new_board[jumps[i].second[j].to], new_board[jumps[i].first]);
            new_board[jumps[i].second[j].through] = ' ';
            genBoards.push_back(new_board);
          }
      }
  return genBoards;
}
vector<BoardType> MinMaxHelper::_generate_boards(BoardType & board, BoardMovesType & moves){
    vector<BoardType> genBoards;

    genBoards.reserve(moves.size());
    BoardType new_board;

    for (size_t i = 0; i < moves.size(); ++i){
        for (size_t j = 0; j < moves[i].second.size(); ++j){
            new_board = board;
            swap(new_board[moves[i].second[j]], new_board[moves[i].first]);
            genBoards.push_back(new_board);
          }
      }
    return genBoards;
  }

int MinMaxHelper::evaluateBoard(BoardType & board){
    int count = 0;
    for (size_t i = 0; i < board.size(); ++i){
        if (board[i] == ' '){
          continue;
        }

        if (isupper(board[i])){
          count += (tolower(board[i]) == player_color) ?
                    KING_WEIGHT: -KING_WEIGHT;
        }
        else{
          count += (board[i] == player_color) ?
                    PAWN_WEIGHT: -PAWN_WEIGHT;
        }
    }
    return count;
}

  void MinMaxHelper::swapActivePlayer(){
      activeGenerator = (active_player == RED) ?
                        blackGenerator:
                        redGenerator;
      swap(active_player, inactive_player);
  }

void MinMaxHelper::setNextJumps(BoardJumpsType & Jumps){
    for (size_t i = 0; i < Jumps.size(); ++i){
        for (size_t j = 0; j < Jumps[i].second.size(); ++j){
            nextJumps.push_back(make_pair(Jumps[i].first, Jumps[i].second[j]));
          }
      }
 }

 void MinMaxHelper::setNextMoves(BoardMovesType & Moves){
    for (size_t i = 0; i < Moves.size(); ++i){
          for (size_t j = 0; j < Moves[i].second.size(); ++j){
              nextMoves.push_back(make_pair(Moves[i].first, Moves[i].second[j]));
            }
        }
    }
