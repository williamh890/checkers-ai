#include "headers/checkers-game.h"
using ai::CheckersGame;
using ai::loadMoveTableFrom;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;

#include "headers/models.h"
using ai::Position;
using ai::Piece;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getGeneratorFor("red", converter);
    auto black = getGeneratorFor("black", converter);

    return CheckersGame(red, black);
}

CheckersGame::CheckersGame(
        MoveGenerator red,
        MoveGenerator black): redGenerator(red), blackGenerator(black) {
    initPieces();
}

void CheckersGame::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (isInFirst3Rows(space)) {
            Piece newPiece = Piece('r', space);
            redPieces.push_back(newPiece);
        }

        if (isInLast3Rows(space)) {
            Piece newPiece = Piece('b', space);
            blackPieces.push_back(newPiece);
        }
    }
}

bool CheckersGame::isInFirst3Rows(int space) {
    return space < (INIT_NUM_PIECES);
}

bool CheckersGame::isInLast3Rows(int space) {
    return space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES);
}

void CheckersGame::printBoard() {
    //string board = "";

    auto spaces = getEmptyBoard();

    for (auto piece: blackPieces) {
        auto pos = spaceToGridSquare(piece.space);

        spaces[pos.row][pos.col] = piece.color;

        for (auto move: blackGenerator.getMoves(piece.space)) {
            auto movePos = spaceToGridSquare(move);
            if (spaces[movePos.row][movePos.col] == piece.color) {
                continue;
            }
            spaces[movePos.row][movePos.col] = 'm';
        }

        for (auto jump: blackGenerator.getJumps(piece.space)) {
            auto to = spaceToGridSquare(jump.to);
            if (spaces[to.row][to.col] == piece.color) {
                continue;
            }
            spaces[to.row][to.col] = 'j';
        }
    }

    for (auto piece: redPieces) {
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
    auto pieces = (color == "black") ? blackPieces : redPieces;

    for (const auto & checker : pieces)
    {
        auto s = spaceToGridSquare(checker.space);
        cout << "(" << s.row << ", " << s.col << "): ";
        for (const auto & move: blackGenerator.getMoves(checker.space)) {
            auto m = spaceToGridSquare(move);
            // cout << "["  << m.row << "," << m.col << "] ";
            cout << move << " ";
        }
        cout << endl;
    }

}

void CheckersGame::printJumpsForColor(const string & color) {
    cout << color << " Jumps: " << endl;
    auto pieces = (color == "black") ? blackPieces : redPieces;

    for (const auto & checker: pieces) {
        auto s = spaceToGridSquare(checker.space);
        // cout << "(" << s.row << ", " << s.col << "):";;
        cout << checker.space << ": ";

        for (const auto & jump: blackGenerator.getJumps(checker.space)) {
            auto to = spaceToGridSquare(jump.to);
            //cout << "[" <<  to.row << "," << to.col << "] ";
            cout << jump.to << ", " << jump.through << " ";
        }
        cout << endl;
    }
}


void CheckersGame::printValidMoves()
{

}



