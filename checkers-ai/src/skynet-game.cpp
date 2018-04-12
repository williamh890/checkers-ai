
#include "checkers-game.h"

#include "search.h"
using ai::SearchHelper;

#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>

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

void skynetPlay(ai::CheckersGame & game) {

}

// Command line args are
//      ./skynet-game.out <game-name> <network-id> <search-depth-limit>
int main(int argc, char **argv) {

    std::string gameName = argv[1];
    unsigned int red_id = std::stoi(argv[2]);
    SearchHelper::limit = std::stof(argv[3]);

    auto game = ai::getNetworkedCheckersGame(red_id, 0);
    std::cout << "game-state: " << game.getSkynetBoardStr() << std::endl;
    std::cout << getGameInfo(gameName) << std::endl;

    return 0;
}
