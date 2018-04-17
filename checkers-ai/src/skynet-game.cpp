
#include "checkers-game.h"

#include "search.h"
using ai::SearchHelper;

#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>

// Command line args are
//      ./skynet-game.out <game-name> <player-color> <network-id> <search-depth-limit>

const std::string server_ = " --server http://localhost:8080 ";
const std::string server = " --server http://skynet.cs.uaf.edu ";

struct GameInfo {
    std::string turn;
    std::string board;
    GameInfo() = default;
    GameInfo(
        const std::string & turn,
        const std::string & board
    ) : turn(turn), board(board) {}
};

void sendTurn(const std::string & gameName, const std::string & board) {
    std::string sendCmd = "python3 pyintheskynet/skynet.py play_game --game " + gameName + " --board " + board;
    sendCmd += server;
    auto ret = system(sendCmd.c_str());
}

GameInfo getGameInfo(const std::string & game) {
    auto cmdOutfile = "gameinfo.txt";
    std::string listCmd = "python3 pyintheskynet/skynet.py game_info --game " + game + " > " + cmdOutfile;
    listCmd += server;
    auto ret = system(listCmd.c_str());

    std::ifstream boardFile(cmdOutfile);
    std::string boardStr = "", turn = "";

    boardFile >> turn >> boardStr;

    return GameInfo(turn, boardStr);
}


std::string formatBoard(std::string board, const char playerColor) {
    for (auto & c : board) {
        if (c == '_') {
            c = ' ';
        }
    }

    return board;
}

std::string getNextBoardState(
        const std::string & gameName,
        const char playerColor) {
    GameInfo stateFromServer;
    size_t timeout = 0;

    do {
        stateFromServer = getGameInfo(gameName);

        if (timeout++ > 10000) {
            std::cout << "Gametimed out..." << std::endl;
            exit(-1);
        }
    } while (stateFromServer.turn[0] != playerColor);

    return formatBoard(stateFromServer.board, playerColor);
}


void skynetPlay(const std::string & gameName, const char playerColor, ai::CheckersGame & game) {
    while (game.moveCounter++ < 100) {
        auto localState = game.getSkynetBoardStr();

        auto newBoard = getNextBoardState(gameName, playerColor);
        game.setState(newBoard);
        game.turn();

        sendTurn(gameName, game.getSkynetBoardStr());
    }
}

int main(int argc, char **argv) {
    std::string gameName = argv[1];
    const char playerColor = std::string(argv[2])[0];
    int network_id = std::stoi(argv[3]);
    ai::CheckersGame::SEARCH_DEPTH = std::stoi(argv[4]);
    ai::CheckersGame game;
    if (network_id < 0) {
        game = (playerColor == 'r') ?
            ai::getCheckersGame():
            ai::getCheckersGame();
    } else {
        game = (playerColor == 'r') ?
            ai::getNetworkedCheckersGame(network_id, 0):
            ai::getNetworkedCheckersGame(0, network_id);
    }

    if (game.getActivePlayerColor() != playerColor) {
        game.swapPlayers();
    }

    skynetPlay(gameName, playerColor, game);

    return 0;
}
