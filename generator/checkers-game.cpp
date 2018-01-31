#include "headers/checkers-game.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::CheckersGame;
using ai::loadMoveTableFrom;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "headers/models.h"
using ai::Jump;
using ai::Position;
using ai::Piece;

#include "headers/consts.h"
using ai::TOTAL_NUM_PIECES;
using ai::INIT_NUM_PIECES;

#include "headers/table-types.h"
using ai::MoveTableType;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <utility>
using std::move;

Player::Player(char color, MoveGenerator generator): color(color), generator(generator) {
    initPieces();
}

void Player::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (isInitialSpace(space)) {
            Piece newPiece = Piece(color, space);
            pieces.push_back(newPiece);
        }
    }
}

vector<Piece> Player::getPieces() const {
    return pieces;
}

char Player::getColor() const {
    return color;
}

vector<int> Player::getMovesFor(Piece piece) const {
    return generator.getMoves(piece.space);
};

vector<Jump> Player::getJumpsFor(Piece piece) const {
    return generator.getJumps(piece.space);
};

BlackPlayer::BlackPlayer(char color, MoveGenerator generator): Player(color, generator) {}

bool BlackPlayer::isInitialSpace(int space) const {
    return space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES);
}

RedPlayer::RedPlayer(char color, MoveGenerator generator): Player(color, generator) {}

bool RedPlayer::isInitialSpace(int space) const {
    return space < (INIT_NUM_PIECES);
}

shared_ptr<Player> getPlayer(const string & color, JsonToStlConverter converter) {
    if (color == "black") {
        auto redGenerator = getGeneratorFor("r", converter);
        return make_shared<RedPlayer>('r', redGenerator);
    }

    auto blackGenerator = getGeneratorFor("b", converter);
    return make_shared<BlackPlayer>('b', blackGenerator);
}

CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);

    return CheckersGame(red, black);
}

CheckersGame::CheckersGame(
        shared_ptr<Player> red,
        shared_ptr<Player> black): red(red), black(black) {
}

void CheckersGame::printBoard() {

    auto spaces = getEmptyBoard();

    for (auto piece: black->getPieces()) {
        auto pos = spaceToGridSquare(piece.space);

        spaces[pos.row][pos.col] = piece.color;

        for (auto move: black->getMovesFor(piece)) {
            auto movePos = spaceToGridSquare(move);
            if (spaces[movePos.row][movePos.col] == piece.color) {
                continue;
            }
            spaces[movePos.row][movePos.col] = 'm';
        }

        for (auto jump: black->getJumpsFor(piece)) {
            auto to = spaceToGridSquare(jump.to);
            if (spaces[to.row][to.col] == piece.color) {
                continue;
            }
            spaces[to.row][to.col] = 'j';
        }
    }

    for (auto piece: red->getPieces()) {
        auto pos = spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    const string boardNums = "     0   1   2   3   4   5   6   7  ";
    const string spacerRow = "   +---+---+---+---+---+---+---+---+";

    cout << boardNums << endl;
    auto rowCounter = 0;
    for ( auto row : spaces ) {
        cout << spacerRow << endl;

        cout << " " << rowCounter << " ";
        for (auto space : row) {
            cout << "| " << space << " ";
        }
        cout << "|" << endl;

        ++rowCounter;
    }
    cout << spacerRow << endl;
}

vector<vector<char>> CheckersGame::getEmptyBoard() {
    vector<vector<char>> board;

    for (auto r = 0; r < ROWS; ++r) {
        auto row = vector<char>(ROWS, ' ');
        board.push_back(row);
    }

    return board;
}

Position CheckersGame::spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}

void CheckersGame::printMoves()
{
    printJumpsForColor("black");
}

void CheckersGame::printMovesForColor(const string & color) {
    cout << color << " Moves: " << endl;
    auto pieces = ((color == "black") ? black: red)->getPieces();

    for (const auto & checker : pieces)
    {
        auto s = spaceToGridSquare(checker.space);
        cout << "(" << s.row << ", " << s.col << "): ";
        for (const auto & move: black->getMovesFor(checker)) {
            auto m = spaceToGridSquare(move);
            // cout << "["  << m.row << "," << m.col << "] ";
            cout << move << " ";
        }
        cout << "]"<<endl;
    }

}

void CheckersGame::printJumpsForColor(const string & color) {
    cout << color << " Jumps: " << endl;
    auto pieces = ((color == "black") ? black: red)->getPieces();

    for (const auto & checker: pieces) {
        auto s = spaceToGridSquare(checker.space);
        // cout << "(" << s.row << ", " << s.col << "):";;
        cout << checker.space << ": ";

        for (const auto & jump: black->getJumpsFor(checker)) {
            auto to = spaceToGridSquare(jump.to);
            //cout << "[" <<  to.row << "," << to.col << "] ";
            cout << jump.to << ", " << jump.through << " ";
        }
        cout << endl;
    }
}


MoveTableType CheckersGame::getBlackMoves()
{
    MoveTableType blackMoves;
    bool valid = true;
    for(const auto & piece : blackPieces)
    {
        auto moves = blackGenerator.getMoves(piece.space);
        vector<int> black_move;
        for(auto & move : moves)
        {
            for (const auto & other_black: blackPieces)
            {
                if(move == other_black.space)
                {
                    valid = false;
                    break;
                }
            }
            if(valid)
            {
                for (const auto & other_red: redPieces)
                {
                    if(move == other_red.space)
                    {
                        valid = false;
                    }
                }
                if(valid)
                {
                    black_move.push_back(piece.space);
                    black_move.push_back(move);
                }
            }
            valid = true;
        }
        if (black_move.size())
        {
            blackMoves.push_back(black_move);
        }
    }
    return blackMoves;
}
