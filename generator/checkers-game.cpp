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

        pair<int, int> move = make_pair(-1, -1);
        pair<int, Jump> jump = make_pair(-1, Jump(-1, -1));

        try {
            if (getValidJumps().size() > 0) {
                jump = getJumpFromActivePlayer();
            } else {
                move = getMoveFromActivePlayer();
            }
        }
        catch(length_error & e) {
            cout << "Out of moves" << endl;
            break;
        }
        catch(runtime_error & e) {
            cout << "Invlaid Move..." << endl;
            continue;
        }

        cout << activePlayer->getColor() << ": "
            << spaceToPosition(move.first).toString() << ", "
            << spaceToPosition(move.second).toString()
            << endl;

        Action action;
        if (move.first == -1) {
            action = board.make(jump);
            reactTo(action, jump);

            if (getValidJumps().size() > 0) {
                continue;
            }
        }
        else {
            action = board.make(move);
            reactTo(action, move);
        }


        swap(activePlayer, inactivePlayer);
    }
}

pair<int, Jump> CheckersGame::getJumpFromActivePlayer() {
    if (activePlayer->getPlayerType() == PlayerType::Computer) {
        return getRandomValidJump();
    }

    return getJumpFromUser();
}

pair<int, Jump> CheckersGame::getRandomValidJump() {
    auto jumps = getValidJumps();

    auto numJumps = jumps.size();
    cout << "number of jumps: " << numJumps << endl;

    return jumps[rand() % numJumps];
}

pair<int, Jump> CheckersGame::getJumpFromUser() {
    auto inputJump = parseUserInput();

    return getJumpFrom(inputJump);
}

pair<int, Jump> CheckersGame::getJumpFrom(const pair<int, int> & inputJump) {
    auto validJumps = getValidJumps();

    for (const auto & checkJump: validJumps) {
        if (checkJump.first == inputJump.first and
            checkJump.second.to == inputJump.second) {

            return checkJump;
        }
    }

    throw runtime_error("invalid jump input.");
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

pair<int, int> CheckersGame::parseUserInput() {
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

    return make_pair(start, end);
}

pair<int, int> CheckersGame::getMoveFromUser() {
    auto move = parseUserInput();

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

vector<pair<int, Jump>> CheckersGame::getValidJumps() {
    auto validJumps = board.getValidJumpsFor(activePlayer);

    return validJumps;
}

// TODO! Want to combine these reactTo functions in the future
void CheckersGame::reactTo(const Action & action, const pair<int, Jump> & jump) {
    if (action == Action::Jump) {
        activePlayer->updatePieces(jump);
        inactivePlayer->removePieceAt(jump.second.through);
        cout << "piece was jumped" << endl;
    }
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
