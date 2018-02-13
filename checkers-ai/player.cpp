#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;
using ai::PlayerType;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getKingGenerator;
using ai::getGeneratorFor;

#include "headers/utils.h"
using ai::spaceToPosition;

#include "headers/table-types.h"
using ai::MoveTableType;

#include "headers/models.h"
using ai::Piece;
using ai::Jump;

#include "headers/consts.h"
using ai::TOTAL_NUM_SPACES;
using ai::INIT_NUM_PIECES;
using ai::NUM_PIECES_IN_ROW;

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <sstream>
using std::stringstream;
#include <iostream>
using std::cout;
using std::endl;
#include <utility>
using std::pair;


Player::Player(
        char color,
        const MoveGenerator & generator,
        const MoveGenerator & kingGenerator,
        PlayerType type=PlayerType::Computer) : color(color), generator(generator), kingGenerator(kingGenerator), playerType(type) {
}

void Player::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_SPACES; ++space) {
        if (isInitialSpace(space)) {
            Piece newPiece = Piece(color, space);
            pieces.push_back(newPiece);
        }
    }
}

const vector<Piece> Player::getPieces() const {
    return pieces;
}

const char Player::getColor() const {
    return color;
}

const PlayerType Player::getPlayerType() const {
    return playerType;
}

void Player::removePieceAt(int space) {
    for (auto s = pieces.size() - 1; s >= 0; --s) {
        auto piece = pieces[s];

        if (piece.space == space) {
            pieces.erase(pieces.begin() + s);
            break;
        }
    }
}

void Player::Crown(Piece & piece)
{
    piece.isKing = true;
    cout << "crowning piece in space: " << piece.space <<endl;
    if(piece.color == 'r')
    {
        piece.color = 'R';
    }
    else
    {
        piece.color = 'B';
    }
}

void Player::updatePieces(const pair<int, int> & move, Board & board) {
    for (auto & piece : pieces) {
        if (piece.space == move.first) {
            piece.space = move.second;

            if(shouldBeCrowned(piece)) {
                Crown(piece);
                board.updatePiece(piece.space, piece.color);
            }

            return;
        }
    }
}

void Player::updatePieces(const pair<int, Jump> & jump, Board & board) {
    for (auto & piece : pieces) {
        if (piece.space == jump.first) {
            piece.space = jump.second.to;

            if(shouldBeCrowned(piece)) {
                Crown(piece);
                board.updatePiece(piece.space, piece.color);
            }

            return;
        }
    }
}

vector<Jump> Player::getJumpsFor(const Piece & piece) const {
    if (piece.isKing) {
        return kingGenerator.getJumps(piece.space);
    }

    return generator.getJumps( piece.space );
}

vector<int> Player::getMovesFor(const Piece & piece) const {
    if (piece.isKing) {
        return kingGenerator.getMoves(piece.space);
    }

    return generator.getMoves( piece.space );
}

string Player::movesToString() const {
    stringstream ss;
    ss << color << " Moves: " << endl;

    for (const auto & checker : pieces) {
        auto s = spaceToPosition(checker.space);

        ss << "(" << s.row << ", " << s.col << "): ";
        for (const auto & move: getMovesFor(checker)) {
            ss << move << " ";
        }
        ss << "]" << endl;
    }

    return ss.str();
}

string Player::jumpsToString() const {
    stringstream ss;
    ss << color << " Jumps: " << endl;

    for (const auto & checker: pieces) {
        ss << checker.space << ": ";

        for (const auto & jump: getJumpsFor(checker)) {
            ss << jump.to << ", " << jump.through << " ";
        }
        ss << endl;
    }

    return ss.str();
}


BlackPlayer::BlackPlayer(
        char color,
        const MoveGenerator & generator,
        const MoveGenerator & kingGenerator,
        PlayerType type): Player(color, generator, kingGenerator, type) {
    initPieces();
}

bool BlackPlayer::isInitialSpace(int space) const {
    return space >= (TOTAL_NUM_SPACES - INIT_NUM_PIECES);
}

bool BlackPlayer::shouldBeCrowned(const Piece & piece) const {
    if (not piece.isKing){
      return piece.space < NUM_PIECES_IN_ROW;
    }
    return false;
}

RedPlayer::RedPlayer(char color,
                     const MoveGenerator & generator,
                     const MoveGenerator & kingGenerator,
                     PlayerType type): Player(color, generator, kingGenerator, type) {
    initPieces();
}

bool RedPlayer::isInitialSpace(int space) const {
    return space < (INIT_NUM_PIECES);
}

bool RedPlayer::shouldBeCrowned(const Piece & piece) const {
    if (not piece.isKing){
      return piece.space >= TOTAL_NUM_SPACES - NUM_PIECES_IN_ROW;
    }
    return false;
}


shared_ptr<Player> ai::getPlayer(const string & color, JsonToStlConverter converter) {
    auto kingGenerator = getKingGenerator(converter);

    if (color == "red") {
        auto redGenerator = getGeneratorFor("red", converter);
        return make_shared<RedPlayer>('r', redGenerator, kingGenerator, Settings::RED_PLAYER_TYPE);
    }

    auto blackGenerator = getGeneratorFor("black", converter);

    return make_shared<BlackPlayer>('b', blackGenerator, kingGenerator, Settings::BLACK_PLAYER_TYPE);
}