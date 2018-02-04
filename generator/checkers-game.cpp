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
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;


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
        shared_ptr<Player> black): board(board), red(red), black(black), activePlayer(black), inactivePlayer(red) {
    random_device device;
    generator = mt19937(device());

    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

void CheckersGame::play() {
    while (moveCounter++ < 100) {
        cout << toString() << endl;

        pair<int, int> move = make_pair(-1, -1);
        pair<int, Jump> jump = make_pair(-1, Jump(-1, -1));

        try {
            if (getValidJumps().size()) {
                jump = getJumpFromActivePlayer();
            } else {
                move = getMoveFromActivePlayer();
            }
        }
        catch(length_error & e) {
            cout << "Out of moves" << endl;
            cout << e.what() << endl;
            break;
        }
        catch(runtime_error & e) {
            cout << "Invalid Move..." << endl;
            cout << e.what() << endl;
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

            auto validJumps = getValidJumpsAt(jump.second.to);

            cout << board.toString() << endl;
            cout << "jump piece has " << validJumps.size() << " possible jumps." << endl;

            while (validJumps.size()) {
                auto jump = make_pair(-1, Jump(-1, -1));

                try {
                    jump = getJumpFromActivePlayer();
                }
                catch(runtime_error & e) {
                    cout << "Invlaid Jump..." << endl;
                    continue;
                }

                action = board.make(jump);
                reactTo(action, jump);

                validJumps = getValidJumpsAt(jump.second.to);
            }
        }
        else {
            action = board.make(move);
            reactTo(action, move);
        }

        swap(activePlayer, inactivePlayer);
    }

    cout << "moves in game " << moveCounter << endl;
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

    uniform_int_distribution<int> distribution(0, numJumps - 1);
    auto rngVal = distribution(generator);
    cout << "rngValue: " << rngVal << endl;
    return jumps[rngVal];
}

pair<int, Jump> CheckersGame::getJumpFromUser() {
    auto inputJump = parseUserInput();

    return getJumpFrom(inputJump);
}

pair<int, Jump> CheckersGame::getJumpFrom(const pair<int, int> & inputJump) {
    auto validJumps = getValidJumps();
    cout << "number of jumps: " << validJumps.size() << endl;

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

    uniform_int_distribution<int> distribution(0, numMoves - 1);
    auto rngVal = distribution(generator);

    return moves[rngVal];
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

    for (auto piece : activePlayer->getPieces()) {
        if (piece.space == move.second) {
            throw runtime_error("cannot move over own piece.");
        }
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
bool CheckersGame::isInvalidJump(const pair<int, Jump> & jump) {
    auto validJumps = getValidJumps();

    for (const auto & validJump : validJumps) {
        if (validJump == jump) {
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

vector<pair<int, Jump>> CheckersGame::getValidJumpsAt(int space) {
    auto validJumps = board.getValidJumpsFor(activePlayer);
    auto jumpsAtSpace = vector<pair<int, Jump>>{};

    for (auto & jump: validJumps) {
        if (jump.first == space) {
            jumpsAtSpace.push_back(jump);
        }
    }

    return jumpsAtSpace;
}

// TODO! Want to combine these reactTo functions in the future
void CheckersGame::reactTo(const Action & action, const pair<int, Jump> & jump) {
    if (action == Action::Jump) {
        activePlayer->updatePieces(jump, board);
        inactivePlayer->removePieceAt(jump.second.through);
        cout << "piece was jumped" << endl;
    }
}

void CheckersGame::reactTo(const Action & action, const pair<int, int> & move) {
    if (action == Action::Move) {
        activePlayer->updatePieces(move, board);
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
