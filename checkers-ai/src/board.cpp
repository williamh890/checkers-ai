#include "board.h"
using ai::Board;

#include "player.h"
using ai::Player;

#include "table-types.h"
using ai::MoveTableType;

#include "utils.h"
using ai::positionToSpace;
using ai::spaceToPosition;

#include "models.h"
using ai::Jump;
using ai::Piece;

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <sstream>
using std::stringstream;
#include <utility>
using std::make_pair;
using std::pair;
#include <algorithm>
using std::swap;
#include <map>
using std::map;
#include <cctype>

Board::Board() : boardState(vector<char>(32, ' ')){};

void Board::addPiecesFor(const shared_ptr<Player> &player) {
  for (const auto &piece : player->getPieces()) {
    boardState[piece.space] = piece.color;
  }
}

map<string, vector<Piece>> Board::getPieces() {
  map<string, vector<Piece>> pieces{{"red", {}}, {"black", {}}};

  for (size_t space = 0; space < boardState.size(); ++space) {
    auto p = boardState[space];
    if (p == ' ') {
      continue;
    }

    auto color = (tolower(p) == 'r') ? "red" : "black";
    auto newPiece = Piece(tolower(p), space);
    newPiece.isKing = (isupper(p) != 0);

    pieces.at(color).push_back(newPiece);
  }

  return pieces;
}

bool Board::hasPieceAt(int space) const { return boardState[space] != ' '; }

const vector<pair<int, Jump>> Board::getValidJumpsFor(
    const shared_ptr<Player> &player) const {
  vector<pair<int, Jump>> validJumps;

  for (const auto &piece : player->getPieces()) {
    auto jumps = player->getJumpsFor(piece);

    for (auto jump : jumps) {
      if (not hasOpposingPieceAt(jump, player->getColor())) {
        continue;
      }
      if (destinationIsNotEmpty(jump)) {
        continue;
      }

      auto validJump = make_pair(piece.space, jump);
      validJumps.push_back(validJump);
    }
  }

  return validJumps;
}

bool Board::hasOpposingPieceAt(const Jump &jump, char color) const {
  return boardState[jump.through] != ' ' and
         tolower(boardState[jump.through]) != tolower(color);
}

bool Board::destinationIsNotEmpty(const Jump &jump) const {
  return hasPieceAt(jump.to);
}

const vector<pair<int, int>> Board::getValidMovesFor(
    const shared_ptr<Player> &player) const {
  vector<pair<int, int>> validMoves;

  for (const auto &piece : player->getPieces()) {
    auto moves = player->getMovesFor(piece);

    for (auto move : moves) {
      if (hasPieceAt(move)) {
        continue;
      }

      auto validMove = make_pair(piece.space, move);
      validMoves.push_back(validMove);
    }
  }

  return validMoves;
}

void Board::make(const pair<int, int> &move) {
  swap(boardState[move.first], boardState[move.second]);
}

void Board::make(const pair<int, Jump> &jump) {
  swap(boardState[jump.second.to], boardState[jump.first]);
  boardState[jump.second.through] = ' ';
}

std::string Board::skynetStr() {
  std::string board = "";

  for (const auto &c : boardState) {
    if (c == ' ') {
      board += '_';
      continue;
    }

    board += c;
  }
  return board;
}

string Board::toString() {
  auto spaces = getEmptyBoard();

  for (auto i = 0; i < static_cast<int>(boardState.size()); ++i) {
    auto pos = spaceToPosition(i);

    spaces[pos.row][pos.col] = boardState[i];
  }

  const string boardNums = "     0   1   2   3   4   5   6   7  ";
  const string spacerRow = "   +---+---+---+---+---+---+---+---+";

  stringstream ss;
  ss << boardNums << endl;
  auto rowCounter = 0;
  for (auto row : spaces) {
    ss << spacerRow << endl;

    ss << " " << rowCounter << " ";
    for (auto space : row) {
      ss << "| " << space << " ";
    }
    ss << "|" << endl;

    ++rowCounter;
  }
  ss << spacerRow << endl;

  return ss.str();
}

Board ai::getBoard() { return Board(); }

std::vector<char> Board::getBoardState() { return boardState; }

void Board::setBoardState(const vector<char> &newState) {
  boardState = newState;
}

char Board::operator[](const int &index) const { return boardState[index]; }

void Board::updatePiece(const int &index, const char &newVal) {
  boardState[index] = newVal;
}

vector<vector<char>> Board::getEmptyBoard() {
  vector<vector<char>> board;

  for (auto r = 0; r < 8; ++r) {
    auto row = vector<char>(8, ' ');
    board.push_back(row);
  }

  return board;
}

int Board::pieceCount(char color) {
  int redTotal = 0, blackTotal = 0;
  for (auto &position : boardState) {
    if (position == 'r' || position == 'R') {
      ++redTotal;
    }
    if (position == 'b' || position == 'B') {
      ++blackTotal;
    }
  }
  if (color == 'r') {
    return redTotal - blackTotal;
  }
  return blackTotal - redTotal;
}
