#include "headers/gui-cpp-interface.h"
using ai::GuiCppInterface;

#include "headers/checkers-game.h"
using ai::CheckersGame;
using MovePackage = CheckersGame::MovePackage;
using JumpPackage = CheckersGame::JumpPackage;

#include "headers/board.h"
using ai::Board;

#include <unistd.h>

#include <vector>
using std::vector;

#include <utility>
using std::make_pair;
using std::pair;
#include <iostream>
using std::cout;
using std::endl;

GuiCppInterface::GuiCppInterface(const CheckersGame &game) : game(game) {}

void GuiCppInterface::play() {
  auto move = make_pair(-1, -1);
  auto jump = make_pair(-1, Jump(-1, -1));
  if (game.areJumps()) {
    jump = game.getJumpFromActivePlayer();
  } else {
    move = game.getMoveFromActivePlayer();
  }
  if (move.first == -1) {
    game.board.make(jump);
    game.reactTo(jump);

    auto validJumps = game.getValidJumpsAt(jump.second.to);

    while (validJumps.size()) {
      auto jump = make_pair(-1, Jump(-1, -1));

      jump = game.getBestJump();

      game.board.make(jump);
      game.reactTo(jump);

      validJumps = game.getValidJumpsAt(jump.second.to);
    }
  } else {
    game.board.make(move);
    game.reactTo(move);
  }

  swapPlayers();
  usleep(10000);
  // game.makeRandomValidAction();
  // usleep(10000);
  // swapPlayers();
}

bool GuiCppInterface::isInvalid(const CheckersGame::MovePackage &move) {
  return game.isInvalid(move);
}

bool GuiCppInterface::isInvalid(const CheckersGame::JumpPackage &jump) {
  return game.isInvalid(jump);
}

Board::BoardType GuiCppInterface::getBoard() {
  return game.board.getBoardState();
}

const char GuiCppInterface::getActivePlayerColor() {
  return game.getActivePlayerColor();
}

const char GuiCppInterface::getInactivePlayerColor() {
  return game.getInactivePlayerColor();
}

void GuiCppInterface::makeJump(const CheckersGame::JumpPackage &jump) {
  game.board.make(jump);
  game.reactTo(jump);
  cout << game.toString() << endl;
  vector<int> move = {jump.first, jump.second.through, jump.second.to};
  game_record.push_back(move);
  if (not game.getValidJumpsAt(jump.second.to).size()) {
    game.swapPlayers();
    cout << game.activePlayer->jumpsToString() << endl;
    cout << game.activePlayer->movesToString() << endl;
    if (game.areJumps()) {
      JumpPackage jump = game.getBestJump();
      game.board.make(jump);
      game.reactTo(jump);
      while (game.getValidJumpsAt(jump.second.to).size()) {
        jump = game.getBestJump(jump.second.to);
        game.board.make(jump);
        game.reactTo(jump);
      }
    } else {
      auto move = game.getBestMove();
      game.board.make(move);
      game.reactTo(move);
    }
    game.swapPlayers();
    cout << game.activePlayer->jumpsToString() << endl;
    cout << game.activePlayer->movesToString() << endl;
  }
  cout << " done with jump, active player is " << game.getActivePlayerColor()
       << endl;
}

void GuiCppInterface::makeMove(const CheckersGame::MovePackage &move) {
  game.board.make(move);
  game.reactTo(move);
  cout << game.toString() << endl;
  vector<int> vec_move = {move.first, move.second};
  game_record.push_back(vec_move);
  game.swapPlayers();
  cout << game.activePlayer->jumpsToString() << endl;
  cout << game.activePlayer->movesToString() << endl;
  if (areJumps()) {
    JumpPackage jump = game.getBestJump();
    game.board.make(jump);
    game.reactTo(jump);
    while (game.getValidJumpsAt(jump.second.to).size()) {
      jump = game.getBestJump(jump.second.to);
      game.board.make(jump);
      game.reactTo(jump);
    }
  } else {
    auto move = game.getBestMove();
    game.board.make(move);
    game.reactTo(move);
  }

  game.swapPlayers();

  cout << " done with move, active player is " << game.getActivePlayerColor()
       << endl;
  cout << game.activePlayer->jumpsToString() << endl;
  cout << game.activePlayer->movesToString() << endl;
}

bool GuiCppInterface::areJumps() { return game.areJumps(); }

bool GuiCppInterface::areMoves() { return game.areMoves(); }

void GuiCppInterface::replayJump(const CheckersGame::JumpPackage &jump) {
  game.board.make(jump);
  game.reactTo(jump);

  if (not game.areJumps()) {
    game.swapPlayers();
  }
}

void GuiCppInterface::replayMove(const CheckersGame::MovePackage &move) {
  game.board.make(move);
  game.reactTo(move);
  game.swapPlayers();
}

void GuiCppInterface::swapPlayers() { game.swapPlayers(); }

vector<vector<int>> GuiCppInterface::getGame() { return game_record; }
