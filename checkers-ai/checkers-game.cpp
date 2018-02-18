#include "headers/checkers-game.h"
using ai::CheckersGame;
using PlayerPtr = CheckersGame::PlayerPtr;
using MovePackage = CheckersGame::MovePackage;
using JumpPackage = CheckersGame::JumpPackage;
using SeederPtr = CheckersGame::SeederPtr;

#include "headers/seeder.h"
using ai::RandomDeviceSeeder;
using ai::Seeder;
using ai::SRandSeeder;
using ai::getSeeder;

#include "headers/minimax.h"
using ai::minimax;

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
using ai::TOTAL_NUM_SPACES;
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
#include <utility>
using std::pair;
using std::make_pair;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <exception>
using std::runtime_error;
using std::length_error;
#include <random>
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
#include <climits>


CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    auto seeder = getSeeder();

    return CheckersGame(board, red, black, seeder);
}

CheckersGame::CheckersGame() { };

CheckersGame::CheckersGame(
        const Board & board,
        PlayerPtr red,
        PlayerPtr black,
        SeederPtr & seeder): board(board), red(red), black(black), activePlayer(black), inactivePlayer(red) {

    generator = mt19937(seeder->get());

    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

void CheckersGame::play() {
    while (moveCounter++ < 100) {
        cout << toString() << endl;

        MovePackage move = make_pair(-1, -1);
        JumpPackage jump = make_pair(-1, Jump(-1, -1));

        try {
            if (getValidJumps().size()) {
                jump = getJumpFromActivePlayer();
            } else {
                move = getMoveFromActivePlayer();
            }
        }
        catch(length_error & e) {
            cout << e.what() << endl;
            break;
        }
        catch(runtime_error & e) {
            cout << e.what() << endl;
            continue;
        }

        cout << activePlayer->getColor() << ": "
            << spaceToPosition(move.first).toString() << ", "
            << spaceToPosition(move.second).toString()
            << endl;

        if (move.first == -1) {
            board.make(jump);
            reactTo(jump);

            auto validJumps = getValidJumpsAt(jump.second.to);

            cout << board.toString() << endl;

            while (validJumps.size()) {
                auto jump = make_pair(-1, Jump(-1, -1));

                try {
                    jump = getJumpFromActivePlayer();
                }
                catch(runtime_error & e) {
                    continue;
                }

                board.make(jump);
                reactTo(jump);

                validJumps = getValidJumpsAt(jump.second.to);
            }
        }
        else {
            board.make(move);
            reactTo(move);
        }

        swapPlayers();
    }

    cout << "moves in game " << moveCounter << endl;
}

void CheckersGame::swapPlayers(){
    swap(activePlayer, inactivePlayer);
}

bool CheckersGame::areJumps(){
    return getValidJumps().size() > 0;
}

bool CheckersGame::areMoves(){
    return getValidMoves().size() > 0;
}
JumpPackage CheckersGame::getJumpFromActivePlayer() {
    if (activePlayer->getColor() == 'r') {
        return getMinimaxJump();
    }

    return getRandomValidJump();
}

MovePackage CheckersGame::getMinimaxMove() {
    int bestMoveVal = INT_MIN;
    MovePackage bestMove;

    for (auto & move : getValidMoves()) {
        auto moveVal = minimax(move, 4, getActivePlayerColor(), *this);
        if (moveVal > bestMoveVal) {
            bestMoveVal = moveVal;
            bestMove = move;
        }
    }

    return bestMove;
}

JumpPackage CheckersGame::getMinimaxJump() {
    int bestJumpVal = INT_MIN;
    JumpPackage bestJump;

    for (auto & jump : getValidJumps()) {
        auto jumpVal = minimax(jump, 4, getActivePlayerColor(), *this);
        if (jumpVal > bestJumpVal) {
            bestJumpVal = jumpVal;
            bestJump = jump;
        }
    }

    return bestJump;
}


JumpPackage CheckersGame::getRandomValidJump() {
    auto jumps = getValidJumps();

    auto numJumps = jumps.size();
    cout << "number of jumps: " << numJumps << endl;

    uniform_int_distribution<int> distribution(0, numJumps - 1);
    auto rngVal = distribution(generator);
    cout << "rngValue: " << rngVal << endl;
    return jumps[rngVal];
}

JumpPackage CheckersGame::getJumpFromUser() {
    auto inputJump = parseUserInput();

    return getJumpFrom(inputJump);
}

JumpPackage CheckersGame::getJumpFrom(const MovePackage & inputJump) {
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

MovePackage CheckersGame::getMoveFromActivePlayer() {
    if (activePlayer->getColor() == 'r') {
        return getMinimaxMove();
    }

    return getRandomValidMove();
}

MovePackage CheckersGame::getRandomValidMove() {
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

MovePackage CheckersGame::parseUserInput() {
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

MovePackage CheckersGame::getMoveFromUser() {
    auto move = parseUserInput();

    for (auto piece : activePlayer->getPieces()) {
        if (piece.space == move.second) {
            throw runtime_error("cannot move over own piece.");
        }
    }

    return move;
}

bool CheckersGame::isInvalid(const MovePackage & move) {
    auto validMoves = getValidMoves();

    for (const auto & validMove : validMoves) {
        if (validMove == move) {
            return false;
        }
    }

    return true;
}

bool CheckersGame::isInvalid(const JumpPackage & jump) {
    auto validJumps = getValidJumps();

    for (const auto & validJump : validJumps) {
        if (validJump == jump) {
            return false;
        }
    }

    return true;
}

vector<MovePackage> CheckersGame::getValidMoves() {
    auto validMoves = board.getValidMovesFor(activePlayer);

    return validMoves;
}

vector<JumpPackage> CheckersGame::getValidJumps() {
    auto validJumps = board.getValidJumpsFor(activePlayer);

    return validJumps;
}

vector<JumpPackage> CheckersGame::getOpponentValidJumps(){
    auto validJumps = board.getValidJumpsFor(inactivePlayer);

    return validJumps;
}
vector<MovePackage> CheckersGame::getOpponentValidMoves(){
    auto validMoves = board.getValidMovesFor(inactivePlayer);

    return validMoves;
}
vector<JumpPackage> CheckersGame::getValidJumpsAt(int space) {
    auto validJumps = board.getValidJumpsFor(activePlayer);
    auto jumpsAtSpace = vector<JumpPackage>{};

    for (auto & jump: validJumps) {
        if (jump.first == space) {
            jumpsAtSpace.push_back(jump);
        }
    }

    return jumpsAtSpace;
}
// Accessor functions for gui/cython wrapper
vector<char> CheckersGame::getBoard(){
    return board.getBoardState();
}

const char CheckersGame::getActivePlayerColor(){
    return activePlayer->getColor();
}

const char CheckersGame::getInactivePlayerColor(){
    return inactivePlayer->getColor();
}

void CheckersGame::reactTo(const JumpPackage & jump) {
    activePlayer->updatePieces(jump, board);
    inactivePlayer->removePieceAt(jump.second.through);
}

void CheckersGame::reactTo(const MovePackage & move) {
    activePlayer->updatePieces(move, board);
}

string CheckersGame::toString() {
    return board.toString();
}


void CheckersGame::makeJump(const JumpPackage & jump){
    try {
        board.make(jump);
        reactTo(jump);
        vector<int> move = {jump.first, jump.second.through, jump.second.to};
        game_record.push_back(move);
        if (not areJumps()){
            swapPlayers();
            makeRandomValidAction();
            swapPlayers();
        }
    } catch(length_error & e) {
        cout << "Out of moves" << endl;
        cout << e.what() << endl;
    }

}

void CheckersGame::makeMove(const MovePackage & move){
    board.make(move);
    reactTo(move);

    vector<int> vec_move = {move.first, move.second};
    game_record.push_back(vec_move);
    swapPlayers();
    makeRandomValidAction();
    swapPlayers();
}

void CheckersGame::makeRandomValidAction(){
    if (areJumps()){
        auto jump = getRandomValidJump();
        board.make(jump);
        reactTo(jump);
        vector<int> move = {jump.first, jump.second.through, jump.second.to};
        game_record.push_back(move);

        if (areJumps()){
            makeRandomValidAction();
        }

        return;
    }

    auto move = getRandomValidMove();
    board.make(move);
    reactTo(move);
    vector<int> vec_move = {move.first, move.second};
    game_record.push_back(vec_move);
}

void CheckersGame::replayJump(const JumpPackage & jump){
    board.make(jump);
    reactTo(jump);
    if (not areJumps()){
        swapPlayers();
    }
}

void CheckersGame::replayMove(const MovePackage & move){
    board.make(move);
    reactTo(move);
    swapPlayers();
}

vector<std::vector<int>> CheckersGame::getGame(){
    return game_record;
}

int CheckersGame::minimaxSearch(Board passedBoard, RedPlayer red_player, BlackPlayer black_player,char playerColor, int depth, int init_depth = 0)
{
    if(depth == init_depth)
    {
        return passedBoard.pieceCount(playerColor);
    }
    auto boardCopy = passedBoard;
    auto redCopy = red_player;
    auto blackCopy = black_player;
    auto shared_black = make_shared<BlackPlayer>(blackCopy);
    auto shared_red = make_shared<RedPlayer>(redCopy);
    vector<MovePackage> player_moves;
    vector<JumpPackage> player_jumps;
    int best_move;
    int loop_move;

    if(playerColor == 'b')
    {
        player_moves = boardCopy.getValidMovesFor(shared_black);
        player_jumps = boardCopy.getValidJumpsFor(shared_black);
    }
    if(playerColor == 'r')
    {
        player_moves = boardCopy.getValidMovesFor(shared_red);
        player_jumps = boardCopy.getValidJumpsFor(shared_red);
    }
    if(player_jumps.size())
    {
        boardCopy.make(player_jumps[0]);
        best_move = minimaxSearch(boardCopy, redCopy, blackCopy, playerColor == 'b' ? 'r' : 'b', depth, init_depth+1);
        for(auto i = 1; i < player_jumps.size(); ++i)
        {
            boardCopy = passedBoard;
            boardCopy.make(player_jumps[i]);
            loop_move = minimaxSearch(boardCopy, redCopy, blackCopy, playerColor == 'b' ? 'r' : 'b', depth, init_depth+1);
            if (best_move < loop_move)
            {
                best_move = loop_move;
            }
        }
        return best_move;
    }
    if(player_moves.size())
    {
        boardCopy.make(player_moves[0]);
        best_move = minimaxSearch(boardCopy, redCopy, blackCopy, playerColor == 'b' ? 'r' : 'b', depth, init_depth+1);
        for(auto i = 1; i < player_moves.size(); ++i)
        {
            boardCopy = passedBoard;
            boardCopy.make(player_moves[i]);
            loop_move = minimaxSearch(boardCopy, redCopy, blackCopy, playerColor == 'b' ? 'r' : 'b', depth, init_depth+1);
            if (best_move < loop_move)
            {
                best_move = loop_move;
            }
        }
        return best_move;
    }
    return 0;
}

int CheckersGame::getNumPiecesFor(char color) {
    return (color == 'r') ? red->getPieces().size() : black->getPieces().size();
}

void CheckersGame::makeMinimaxMove(int depth)
{
    auto current_player = getActivePlayerColor();
}
