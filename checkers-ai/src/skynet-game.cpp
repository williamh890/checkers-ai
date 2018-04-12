
#include "checkers-game.h"

#include "search.h"
using ai::SearchHelper;

#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>

void sendTurn(const std::string & gameName, const std::string & board) {
    std::string sendCmd = "python3 pyintheskynet/skynet.py play_game --game " + gameName + " --board " + board;
    auto ret = system(sendCmd.c_str());
}

std::string getGameInfo(const std::string & game) {
    auto cmdOutfile = "gameinfo.txt";
    std::string listCmd = "python3 pyintheskynet/skynet.py game_info --game " + game + " > " + cmdOutfile;
    auto ret = system(listCmd.c_str());

    std::ifstream boardFile(cmdOutfile);
    std::string gameState = "", x = "";

    while (boardFile >> x) {
        gameState  += x;
    }

    return gameState;
}


std::string formatBoard(std::string board) {
    for (auto & c : board) {
        if (c == '_') {
            c = ' ';
        }
    }

    return board;
}

std::string getNextBoardState(
        const std::string & gameName,
        const std::string & localState) {
    std::string stateFromServer;
    size_t timeout = 0;

    do {
        stateFromServer = getGameInfo(gameName);

        if (timeout++ > 10000) {
            std::cout << "Gametimed out..." << std::endl;
            exit(-1);
        }
    } while (stateFromServer == localState);

    return formatBoard(stateFromServer);
}


void skynetPlay(const std::string & gameName, ai::CheckersGame & game) {
    while (game.moveCounter++ < 100) {
        auto localState = game.getSkynetBoardStr();

        auto newBoard = getNextBoardState(gameName, localState);
        game.setState(newBoard);
        game.turn();

        sendTurn(gameName, game.getSkynetBoardStr());

        break;
    }

    std::cout << "game-state: " << game.getSkynetBoardStr() << std::endl;
    std::cout << getGameInfo(gameName) << std::endl;
}

// Command line args are
//      ./skynet-game.out <game-name> <network-id> <search-depth-limit>
int main(int argc, char **argv) {

    std::string gameName = argv[1];
    unsigned int red_id = std::stoi(argv[2]);
    SearchHelper::limit = std::stof(argv[3]);

    auto game = ai::getNetworkedCheckersGame(red_id, 0);

    skynetPlay(gameName, game);

    return 0;
}
