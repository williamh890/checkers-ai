#include "../headers/checkers-game.h"
using ai::CheckersGame;
using PlayerPtr = CheckersGame::PlayerPtr;
using MovePackage = CheckersGame::MovePackage;
using JumpPackage = CheckersGame::JumpPackage;
using SeederPtr = CheckersGame::SeederPtr;

#include "../headers/seeder.h"
using ai::getSeeder;
using ai::RandomDeviceSeeder;
using ai::Seeder;
using ai::SRandSeeder;

#include "../headers/search.h"

#include "../headers/board.h"
using ai::Board;
using ai::getBoard;

#include "../headers/player.h"
using ai::BlackPlayer;
using ai::getNetworkedPlayer;
using ai::getPlayer;
using ai::Player;
using ai::RedPlayer;

#include "../headers/json-to-stl.h"

#include "../headers/move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "../headers/utils.h"
using ai::getTime;

#include "../headers/models.h"
using ai::Jump;
using ai::Piece;
using ai::Position;

#include "../headers/game-state.h"
using ai::GameState;

#include "../headers/consts.h"
using ai::INIT_NUM_PIECES;
using ai::MOVE_LIMIT;
using ai::TOTAL_NUM_SPACES;

#include "../headers/table-types.h"
using ai::MoveTableType;

#include <algorithm>
using std::sort;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <utility>
using std::make_pair;
using std::pair;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <exception>
using std::length_error;
using std::runtime_error;
#include <random>
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

int CheckersGame::SEARCH_DEPTH = 3;

CheckersGame ai::getCheckersGame() {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = getPlayer("red", converter);
  auto black = getPlayer("black", converter);
  auto board = getBoard();

  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getNetworkedCheckersGame(unsigned int red_id,
                                          unsigned int black_id) {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = getNetworkedPlayer("red", converter, red_id);
  auto black = getNetworkedPlayer("black", converter, black_id);

  auto board = getBoard();
  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getNetworkVPieceCountCheckersGame(unsigned int network_id,
                                                   char networked_player) {
  auto table = loadMoveTableFrom("move-table.json");
  auto converter = JsonToStlConverter{table};

  auto red = (networked_player == 'r')
                 ? getNetworkedPlayer("red", converter, network_id)
                 : getPlayer("red", converter);
  auto black = (networked_player == 'b')
                   ? getNetworkedPlayer("black", converter, network_id)
                   : getPlayer("black", converter);

  auto board = getBoard();
  auto seeder = getSeeder();

  return CheckersGame(board, red, black, seeder);
}

CheckersGame::CheckersGame(){};

CheckersGame::CheckersGame(const Board &board, PlayerPtr red, PlayerPtr black,
                           SeederPtr &seeder)
    : board(board), red(red), black(black), activePlayer(black),
      inactivePlayer(red) {

  generator = mt19937(seeder->get());

  this->board.addPiecesFor(red);
  this->board.addPiecesFor(black);
}

const char CheckersGame::play() {
  while (++moveCounter < MOVE_LIMIT && (areMoves() || areJumps())) {
    try {
      auto start = getTime();
      turn();
      auto end = getTime();

      cout << "turn: " << end - start << endl;
    } catch (length_error &e) {
      break;
    } catch (runtime_error &e) {
      continue;
    }
  }

  return getInactivePlayerColor();
}

void CheckersGame::turn() {
  MovePackage move = make_pair(-1, -1);
  JumpPackage jump = make_pair(-1, Jump(-1, -1));

  if (areJumps()) {
    jump = getJumpFromActivePlayer();
  } else {
    move = getMoveFromActivePlayer();
  }

  if (move.first == -1) {
    board.make(jump);
    reactTo(jump);

    makeAnyMultiJumps(jump.second.to);
  } else {
    board.make(move);
    reactTo(move);
  }

  swapPlayers();
}

void CheckersGame::makeAnyMultiJumps(int space) {
  auto validJumps = getValidJumpsAt(space);

  while (validJumps.size()) {
    auto jump = make_pair(-1, Jump(-1, -1));

    try {
      jump = getBestJump();
    } catch (runtime_error &e) {
      continue;
    }

    board.make(jump);
    reactTo(jump);

    validJumps = getValidJumpsAt(jump.second.to);
  }
}

void CheckersGame::swapPlayers() { swap(activePlayer, inactivePlayer); }

bool CheckersGame::areJumps() { return getValidJumps().size() > 0; }

bool CheckersGame::areMoves() { return getValidMoves().size() > 0; }

JumpPackage CheckersGame::getJumpFromActivePlayer() {
  return this->getBestJump();
}

MovePackage CheckersGame::getBestMove() {
#ifdef USE_IDS
  return ai::getBestMoveIDS(*this);
#else
  return ai::getBestMove(*this, CheckersGame::SEARCH_DEPTH);
#endif
}

JumpPackage CheckersGame::getBestJump(int space) {
#ifdef USE_IDS
  return ai::getBestJumpIDS(*this, space);
#else
  return ai::getBestJump(*this, CheckersGame::SEARCH_DEPTH, space);
#endif
}

JumpPackage CheckersGame::getRandomValidJump() {
  auto jumps = getValidJumps();

  auto numJumps = jumps.size();

  uniform_int_distribution<int> distribution(0, numJumps - 1);
  auto rngVal = distribution(generator);
  return jumps[rngVal];
}

MovePackage CheckersGame::getMoveFromActivePlayer() { return getBestMove(); }

MovePackage CheckersGame::getRandomValidMove() {
  auto moves = getValidMoves();
  auto numMoves = moves.size();

  if (numMoves < 1) {
    throw length_error("out of moves");
  }

  uniform_int_distribution<int> distribution(0, numMoves - 1);
  auto rngVal = distribution(generator);

  return moves[rngVal];
}

bool CheckersGame::isInvalid(const MovePackage &move) {
  auto validMoves = getValidMoves();

  for (const auto &validMove : validMoves) {
    if (validMove == move) {
      return false;
    }
  }

  return true;
}

bool CheckersGame::isInvalid(const JumpPackage &jump) {
  auto validJumps = getValidJumps();

  for (const auto &validJump : validJumps) {
    if (validJump == jump) {
      return false;
    }
  }

  return true;
}

vector<MovePackage> CheckersGame::getValidMoves() {
  auto validMoves = board.getValidMovesFor(activePlayer);

  return validMoves;
}

vector<JumpPackage> CheckersGame::getValidJumps() {
  auto validJumps = board.getValidJumpsFor(activePlayer);

  return validJumps;
}

vector<JumpPackage> CheckersGame::getValidJumpsAt(int space) {
  auto validJumps = board.getValidJumpsFor(activePlayer);
  auto jumpsAtSpace = vector<JumpPackage>{};

  for (auto &jump : validJumps) {
    if (jump.first == space) {
      jumpsAtSpace.push_back(jump);
    }
  }

  return jumpsAtSpace;
}

string CheckersGame::validMovesToString() {
  stringstream ss;
  auto validMoves = getValidMoves();
  sort(validMoves.begin(), validMoves.end());
  int from = validMoves[0].first;
  ss << activePlayer->getColor() << " Moves: "
     << "\n"
     << from << ": [";

  for (const auto move : validMoves) {
    if (from != move.first) {
      from = move.first;
      ss << "]\n";
      ss << from << ": [";
    }
    ss << move.second << " ";
  }
  ss << "]\n";
  return ss.str();
}

string CheckersGame::validJumpsToString() {
  stringstream ss;
  auto validJumps = getValidJumps();
  sort(validJumps.begin(), validJumps.end(),
       [&](auto jump1, auto jump2) { return jump1.first < jump2.first; });
  int from = validJumps[0].first;
  ss << activePlayer->getColor() << " Jumps: \n" << from << ": [";

  for (const auto jump : validJumps) {
    if (from != jump.first) {
      from = jump.first;
      ss << "]\n";
      ss << jump.first << ": [ ";
    }
    ss << "(" << jump.second.through << ", " << jump.second.to << ") ";
  }

  return ss.str();
}
const char CheckersGame::getActivePlayerColor() {
  return activePlayer->getColor();
}

const char CheckersGame::getInactivePlayerColor() {
  return inactivePlayer->getColor();
}

bool CheckersGame::reactTo(const JumpPackage &jump) {
  auto wasPieceCrowned = activePlayer->updatePieces(jump, board);

  inactivePlayer->removePieceAt(jump.second.through);

  return wasPieceCrowned;
}

bool CheckersGame::reactTo(const MovePackage &move) {
  auto wasPieceCrowned = activePlayer->updatePieces(move, board);

  return wasPieceCrowned;
}

string CheckersGame::toString() { return board.toString(); }

GameState CheckersGame::getState() {
  return GameState(board.getBoardState(), red->getPieces(), black->getPieces(),
                   activePlayer->getColor());
}

void CheckersGame::setState(GameState &state) {
  board.setBoardState(state.boardState);
  red->setPieces(state.redPieces);
  black->setPieces(state.blackPieces);
  activePlayer = state.activePlayerColor == 'r' ? red : black;
  inactivePlayer = state.activePlayerColor == 'r' ? black : red;
}

void CheckersGame::makeRandomValidAction() {
  if (areJumps()) {
    auto jump = getRandomValidJump();
    board.make(jump);
    reactTo(jump);

    if (getValidJumpsAt(jump.second.to).size()) {
      makeRandomValidAction();
    }

    return;
  }

  auto move = getRandomValidMove();
  board.make(move);
  reactTo(move);
}

int CheckersGame::getNumPiecesFor(char color) {
  return (color == 'r') ? red->getPieces().size() : black->getPieces().size();
}
