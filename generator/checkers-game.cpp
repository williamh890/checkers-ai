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

#include "headers/table-types.h"
using ai::MoveTableType;

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

    auto red = getGeneratorFor("black", converter);
    auto black = getGeneratorFor("red", converter);

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

    auto spaces = getEmptyBoard();

    for (auto piece: blackPieces) {
        auto pos = spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
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
    cout << "Black Moves: " << endl;
    auto blackMoves = getBlackMoves();
    for (auto move : blackMoves)
    {
        cout << move[0] << ": [ ";
        for(auto i=1;i<move.size();++i)
        {
            cout << i << " ";
        }
        cout << "]"<<endl;
    }
    // cout << "Black Moves: " << endl;
    // for ( const auto & checker : blackPieces)
    // {
    //     auto s = spaceToGridSquare(checker.space);
    //     cout << "(" << s.row << ", " << s.col << "): " ;
    //     for (const auto & move: blackGenerator.getMoves(checker.space)) {
    //         auto m = spaceToGridSquare(move);
    //         cout << "["  << m.row << "," << m.col << "] ";
    //     }
    //     cout << endl;
    // }

    // cout << "Black Jumps: " << endl;
    // for ( Piece checker : blackPieces)
    // {
    //     auto pos = spaceToGridSquare(checker.space);
    //     cout << "\t(" <<pos.row << "," << pos.col << "): ";

    //     for (const auto & jump : blackGenerator.getJumps(checker.space))
    //     {
    //         cout << "[" << jump.to << "," << jump.from << "] ";
    //     }
    //     cout << endl;
    // }
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



