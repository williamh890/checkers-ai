#include "search.h"
using ai::SearchHelper;

#include "player.h"
using ai::BlackPlayer;
using ai::getNetworkedPlayer;
using ai::getPlayer;
using ai::Player;
using ai::PlayerType;
using ai::RedPlayer;

#include "json-to-stl.h"
using ai::JsonToStlConverter;

#include "move-generator.h"
using ai::getGeneratorFor;
using ai::getKingGenerator;

#include "utils.h"
using ai::spaceToPosition;

#include "table-types.h"
using ai::MoveTableType;

#include "models.h"
using ai::Jump;
using ai::Piece;

#include "consts.h"
using ai::INIT_NUM_PIECES;
using ai::NUM_PIECES_IN_ROW;
using ai::TOTAL_NUM_SPACES;

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <sstream>
using std::stringstream;
#include <iostream>
using std::cout;
using std::endl;
#include <utility>
using std::pair;
#include <functional>
using std::function;

Player::Player(char color, const MoveGenerator &generator,
               const MoveGenerator &kingGenerator,
               PlayerType type = PlayerType::Computer)
    : playerType(type),
      color(color),
      generator(generator),
      kingGenerator(kingGenerator) {
  this->baseCase = [&](SearchHelper &helper) -> int {
    ++SearchHelper::leafNodes;
    auto numPieces = helper.game.getNumPiecesFor(helper.maximizingPlayer);

    char opponentColor = (helper.maximizingPlayer == 'r') ? 'b' : 'r';
    auto numEnemyPieces = helper.game.getNumPiecesFor(opponentColor);

    return numPieces - numEnemyPieces;
  };
}

Player::Player(char color, const MoveGenerator &generator,
               const MoveGenerator &kingGenerator, Network &network,
               PlayerType type = PlayerType::Computer)
    : playerType(type),
      color(color),
      network(network),
      generator(generator),
      kingGenerator(kingGenerator) {
  this->baseCase = [&](SearchHelper &helper) -> float {
    const vector<char> board = helper.game.board.getBoardState();
    int colorFactor = (helper.maximizingPlayer == 'r') ? 1 : -1;

    float value = this->network.evaluateBoard(board, false, colorFactor);

    return value;
  };
}

void Player::initPieces() {
  for (auto space = 0; space < TOTAL_NUM_SPACES; ++space) {
    if (isInitialSpace(space)) {
      Piece newPiece = Piece(color, space);
      pieces.push_back(newPiece);
    }
  }
}

const vector<Piece> Player::getPieces() const { return pieces; }

void Player::setPieces(const vector<Piece> &newPieces) { pieces = newPieces; }

const char Player::getColor() const { return color; }

const PlayerType Player::getPlayerType() const { return playerType; }

void Player::removePieceAt(int space) {
  for (auto s = static_cast<int>(pieces.size()) - 1; s >= 0; --s) {
    auto piece = pieces[s];

    if (piece.space == space) {
      pieces.erase(pieces.begin() + s);
      break;
    }
  }
}

void Player::Crown(Piece &piece) {
  piece.isKing = true;
  piece.color = toupper(piece.color);
}

bool Player::updatePieces(const pair<int, int> &move, Board &board) {
  auto wasCrowned = false;
  for (auto &piece : pieces) {
    if (piece.space == move.first) {
      piece.space = move.second;

      if (shouldBeCrowned(piece)) {
        Crown(piece);
        board.updatePiece(piece.space, piece.color);

        wasCrowned = true;
      }

      break;
    }
  }

  return wasCrowned;
}

bool Player::updatePieces(const pair<int, Jump> &jump, Board &board) {
  auto wasCrowned = false;
  for (auto &piece : pieces) {
    if (piece.space == jump.first) {
      piece.space = jump.second.to;

      if (shouldBeCrowned(piece)) {
        Crown(piece);
        board.updatePiece(piece.space, piece.color);
        wasCrowned = true;
      }

      break;
    }
  }

  return wasCrowned;
}

vector<Jump> Player::getJumpsFor(const Piece &piece) const {
  if (piece.isKing) {
    return kingGenerator.getJumps(piece.space);
  }

  return generator.getJumps(piece.space);
}

vector<int> Player::getMovesFor(const Piece &piece) const {
  if (piece.isKing) {
    return kingGenerator.getMoves(piece.space);
  }

  return generator.getMoves(piece.space);
}

string Player::movesToString() const {
  stringstream ss;
  ss << color << " Moves: " << endl;

  for (const auto &checker : pieces) {
    auto s = spaceToPosition(checker.space);

    ss << "(" << s.row << ", " << s.col << "): ";
    for (const auto &move : getMovesFor(checker)) {
      ss << move << " ";
    }
    ss << "]" << endl;
  }

  return ss.str();
}

string Player::jumpsToString() const {
  stringstream ss;
  ss << color << " Jumps: " << endl;

  for (const auto &checker : pieces) {
    ss << checker.space << ": ";

    for (const auto &jump : getJumpsFor(checker)) {
      ss << jump.to << ", " << jump.through << " ";
    }
    ss << endl;
  }

  return ss.str();
}

BlackPlayer::BlackPlayer(char color, const MoveGenerator &generator,
                         const MoveGenerator &kingGenerator, PlayerType type)
    : Player(color, generator, kingGenerator, type) {
  initPieces();
}

BlackPlayer::BlackPlayer(char color, const MoveGenerator &generator,
                         const MoveGenerator &kingGenerator, Network &network,
                         PlayerType type)
    : Player(color, generator, kingGenerator, network, type) {
  initPieces();
}

bool BlackPlayer::isInitialSpace(int space) const {
  return space >= (TOTAL_NUM_SPACES - INIT_NUM_PIECES);
}

bool BlackPlayer::shouldBeCrowned(const Piece &piece) const {
  if (not piece.isKing) {
    return piece.space < NUM_PIECES_IN_ROW;
  }

  return false;
}

RedPlayer::RedPlayer(char color, const MoveGenerator &generator,
                     const MoveGenerator &kingGenerator, PlayerType type)
    : Player(color, generator, kingGenerator, type) {
  initPieces();
}

RedPlayer::RedPlayer(char color, const MoveGenerator &generator,
                     const MoveGenerator &kingGenerator, Network &network,
                     PlayerType type)
    : Player(color, generator, kingGenerator, network, type) {
  initPieces();
}

bool RedPlayer::isInitialSpace(int space) const {
  return space < (INIT_NUM_PIECES);
}

bool RedPlayer::shouldBeCrowned(const Piece &piece) const {
  if (not piece.isKing) {
    return piece.space >= TOTAL_NUM_SPACES - NUM_PIECES_IN_ROW;
  }

  return false;
}

shared_ptr<Player> ai::getPlayer(const string &color,
                                 JsonToStlConverter converter) {
  auto kingGenerator = getKingGenerator(converter);

  if (color == "red") {
    auto redGenerator = getGeneratorFor("red", converter);
    return make_shared<RedPlayer>('r', redGenerator, kingGenerator,
                                  Settings::RED_PLAYER_TYPE);
  }

  auto blackGenerator = getGeneratorFor("black", converter);

  return make_shared<BlackPlayer>('b', blackGenerator, kingGenerator,
                                  Settings::BLACK_PLAYER_TYPE);
}
shared_ptr<Player> ai::getNetworkedPlayer(const string &color,
                                          JsonToStlConverter converter,
                                          uint network_id) {
  auto kingGenerator = getKingGenerator(converter);
  Network network = Network(network_id);
  if (color == "red") {
    auto redGenerator = getGeneratorFor("red", converter);
    return make_shared<RedPlayer>('r', redGenerator, kingGenerator, network,
                                  Settings::RED_PLAYER_TYPE);
  }

  auto blackGenerator = getGeneratorFor("black", converter);

  return make_shared<BlackPlayer>('b', blackGenerator, kingGenerator, network,
                                  Settings::BLACK_PLAYER_TYPE);
}
