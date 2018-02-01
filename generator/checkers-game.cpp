#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/board.h"
using ai::Board;

#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;

#include "headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "headers/utils.h"
using ai::spaceToPosition;

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
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <utility>
using std::pair;
using std::make_pair;
#include <exception>
using std::runtime_error;
using std::length_error;
#include <stdlib.h>     // srand, rand
#include <time.h>       // time


CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    return CheckersGame(board, red, black);
}

CheckersGame::CheckersGame(){ };

CheckersGame::CheckersGame(
        const Board & board,
        shared_ptr<Player> red,
        shared_ptr<Player> black
        ): board(board), red(red), black(black), activePlayer(black), inactivePlayer(red) {
    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);

    srand(time(NULL));
}

void CheckersGame::play() {
    while (true) {
        cout << toString() << endl;

        pair<int, int> move;

        try {
            move = getMoveFromActivePlayer();
        }
        catch(length_error & e) {
            cout << "Out of moves" << endl;
            break;
        }
        catch(runtime_error & e) {
            cout << "Invlaid Move..." << endl;
            continue;
        }

        cout << activePlayer->getColor() << ": " << spaceToPosition(move.first).toString() << ", " << spaceToPosition(move.second).toString() << endl;

        auto action = board.make(move);
        reactTo(action, move);

        swap(activePlayer, inactivePlayer);
    }

}

pair<int, int> CheckersGame::getMoveFromActivePlayer() {
    if (activePlayer->getPlayerType() == PlayerType::Computer) {
        return getRandomValidMove();
    }

    return getMoveFromUser();
}

pair<int, int> CheckersGame::getRandomValidMove() {
    auto moves = getValidMoves();
    auto numMoves = moves.size();

    cout << "number of moves: " << numMoves << endl;
    if ( numMoves < 1 ) {
        throw length_error("out of moves");
    }

    return moves[rand() % numMoves];
}

pair<int, int> CheckersGame::getMoveFromUser() {
    cout << "Enter move (startRow startCol endRow endCol): ";

    string in;
    getline(cin, in);
    istringstream iss(in);

    int startRow, startCol, endRow, endCol;
    iss >> startRow >> startCol >> endRow >> endCol;

    auto startPos = Position(startRow, startCol);
    auto endPos = Position(endRow, endCol);

    auto start = positionToSpace(startPos);
    auto end = positionToSpace(endPos);

    auto move = make_pair(start, end);

    if (isInvalid(move)) {
        throw runtime_error("move is not valid");
    }

    return move;
}

bool CheckersGame::isInvalid(const pair<int, int> & move) {
    auto validMoves = getValidMoves();

    for (const auto & validMove : validMoves) {
        if (validMove == move) {
            return false;
        }
    }

    return true;
}

vector<pair<int, int>> CheckersGame::getValidMoves() {
    auto validMoves = board.getValidMovesFor(activePlayer);

    return validMoves;
}

void CheckersGame::reactTo(const Action & action, const pair<int, int> & move) {
    if (action == Action::Move) {
        activePlayer->updatePieces(move);
        cout << "piece was moved" << endl;
    }
}

string CheckersGame::toString() {
    return board.toString();
}

//returns vector<vector<int with format {piece space, valid move, valid move}
MoveTableType CheckersGame::getBlackMoves()
{
    MoveTableType blackMoves;

    for(const auto & piece : black->getPieces())
    {
        auto moves = black->getMovesFor(piece);
        cout << "debug: moves size:" <<moves.size() <<endl;//debug
        vector<int> black_move;
        black_move.push_back(piece.space);
        for(const auto & move : moves)
        {
            cout << "debug: move" << move <<endl;//debug
            if(board[move] == ' ')
            {
                black_move.push_back(move);
            }
        }
        if (black_move.size()>1)
        {
            blackMoves.push_back(black_move);
        }
    }

    return blackMoves;
}

MoveTableType CheckersGame::getRedMoves()
{
    MoveTableType redmoves;
    return redmoves;
}
