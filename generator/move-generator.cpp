#include "headers/move-generator.h"
using ai::MoveGenerator;
using ai::CheckersGame;
using ai::getCheckersGame;
#include "headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;
#include "headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <utility>
using std::pair;
using std::make_pair;
#include <stdio.h>

ai::MoveGenerator::MoveGenerator(
        const MoveTableType & moves,
        const JumpTableType & jumps): moves(moves),
    jumps(jumps) {
    }

auto ai::MoveGenerator::getMoves(int space) {
    return this->moves[ space ];
}

auto ai::MoveGenerator::getJumps(int space) {
    return this->jumps[ space ];
}

auto ai::getGeneratorFor(const string & color, const JsonToStlConverter & converter) {
    auto moves = converter.getMovesFor(color);
    auto jumps = converter.getJumpsFor(color);

    return MoveGenerator(moves, jumps);
}

ai::Piece::Piece(char color, int space): color(color), space(space)  {}

ai::Position::Position(int r, int c): row(r), col(c) {}

ai::CheckersGame::CheckersGame(
        MoveGenerator red,
        MoveGenerator black): redGenerator(red),
    blackGenerator(black) {
        this->initPieces();
    }

void ai::CheckersGame::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (this->isInFirst3Rows(space)) {
            Piece newPiece = Piece('r', space);
            this->redPieces.push_back(newPiece);
        }

        if (this->isInLast3Rows(space)) {
            Piece newPiece = Piece('b', space);
            this->blackPieces.push_back(newPiece);
        }
    }
}

bool ai::CheckersGame::isInFirst3Rows(int space) {
    return space < (this->INIT_NUM_PIECES);
}

bool ai::CheckersGame::isInLast3Rows(int space) {
    return space >= (this->TOTAL_NUM_PIECES - this->INIT_NUM_PIECES);
}

string ai::CheckersGame::printBoard() {
    string board = "";

    auto spaces = this->getEmptyBoard();

    for (auto piece: this->blackPieces) {
        auto pos = this->spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    for (auto piece: redPieces) {
        auto pos = this->spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    string spacerRow = "+---+---+---+---+---+---+---+---+";
    for ( auto row : spaces ) {
        cout << spacerRow << endl;

        for (auto space : row) {
            cout << "| " << space << " ";
        }
        cout << "|" << endl;

    }
    cout << spacerRow << endl;

    return board;
}

vector<vector<char>> ai::CheckersGame::getEmptyBoard() {
    vector<vector<char>> board;

    for (auto r = 0; r < this->ROWS; ++r) {
        auto row = vector<char>(this->ROWS, ' ');
        board.push_back(row);
    }

    return board;
}

ai::Position ai::CheckersGame::spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return ai::Position(row, (2 * col) + offset);
}

CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getGeneratorFor("black", converter);
    auto black = getGeneratorFor("red", converter);

    return CheckersGame(red, black);
}

