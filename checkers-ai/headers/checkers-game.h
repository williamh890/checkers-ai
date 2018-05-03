#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "board.h"
// ai::Board
#include "seeder.h"
// ai::Seeder
#include "player.h"
// ai::Player
#include "game-state.h"
// ai::GameState

#include <string>
// std::string;
#include <memory>
// std::shared_ptr
#include <vector>
// std::vector
#include <utility>
// std::pair
#include <random>
// std::mt19937

namespace ai {
class CheckersGame {
 public:
  using MovePackage = std::pair<int, int>;
  using JumpPackage = std::pair<int, Jump>;
  using PlayerPtr = std::shared_ptr<Player>;
  using SeederPtr = std::shared_ptr<Seeder>;

  static int SEARCH_DEPTH;
  int moveCounter = 0;

  Board board;

  PlayerPtr red;
  PlayerPtr black;

  PlayerPtr activePlayer;
  PlayerPtr inactivePlayer;

  std::mt19937 generator;

 public:
  CheckersGame();
  CheckersGame(const Board &board, PlayerPtr red, PlayerPtr black,
               SeederPtr &seeder);

  const char play();
  void turn();
  void makeAnyMultiJumps(int space);

  void makeRandomValidAction();

  bool isInvalid(const MovePackage &move);
  bool isInvalid(const JumpPackage &jump);

  const char getActivePlayerColor();
  const char getInactivePlayerColor();

  void swapPlayers();
  bool areJumps();
  bool areMoves();

  std::vector<MovePackage> getValidMoves();
  std::vector<JumpPackage> getValidJumps();
  std::vector<JumpPackage> getValidJumpsAt(int space);

  std::string validMovesToString();
  std::string validJumpsToString();

  GameState getState();
  std::string getSkynetBoardStr();
  void setState(const std::string &boardStr);
  void setState(GameState &state);

  MovePackage getMoveFromActivePlayer();

  MovePackage getRandomValidMove();
  MovePackage getBestMove();

  JumpPackage getJumpFromActivePlayer();

  JumpPackage getRandomValidJump();
  JumpPackage getBestJump(int space = -1);

  bool reactTo(const MovePackage &move);
  bool reactTo(const JumpPackage &jump);

  std::string toString();

  int getNumPiecesFor(char color);
};

}  // namespace ai
#endif  // CHECKERS_GAME_H
