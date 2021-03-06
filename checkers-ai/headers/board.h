#ifndef BOARD_H
#define BOARD_H

#include "player.h"
// ai::Player
#include "table-types.h"
// ai::MoveTableType
#include "models.h"
// ai::pos
// ai::Piece

#include <vector>
// std::vector
#include <map>
// std::map
#include <string>
// std::string
#include <memory>
// std::shared_ptr
#include <utility>
// std::pair

namespace ai {
enum class Action { Move, Jump };

class Player;  // forward declaration
class Board {
 public:
  using BoardType = std::vector<char>;

 private:
  BoardType boardState;

 public:
  Board();
  std::map<std::string, std::vector<Piece>> getPieces();
  void addPiecesFor(const std::shared_ptr<Player>& player);
  void updatePiece(const int& index, const char& newVal);

  const std::vector<std::pair<int, int>> getValidMovesFor(
      const std::shared_ptr<Player>& player) const;
  const std::vector<std::pair<int, Jump>> getValidJumpsFor(
      const std::shared_ptr<Player>& player) const;

  void make(const std::pair<int, Jump>& jump);
  void make(const std::pair<int, int>& move);

 private:
  bool hasPieceAt(int space) const;
  bool hasOpposingPieceAt(const Jump& jump, char color) const;
  bool destinationIsNotEmpty(const Jump& jump) const;

 public:
  std::string toString();
  std::string skynetStr();
  char operator[](const int& index) const;
  int pieceCount(char color);

  std::vector<char> getBoardState();
  void setBoardState(const std::vector<char>& newboardState);

 private:
  std::vector<std::vector<char>> getEmptyBoard();
};

Board getBoard();
}  // namespace ai

#endif  // BOARD_H
