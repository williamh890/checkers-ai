#include "checkers-game.h"
using ai::CheckersGame;

#include "get-checkers-game.h"
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "network.h"
using ai::Network;
using ai::weightChangeOut;

#include "utils.h"
using ai::getTime;

#include "search.h"
using ai::SearchHelper;

#include "consts.h"
using ai::MOVE_LIMIT;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::stof;
using std::stoi;
using std::string;

int main(int argc, char **argv) {
    //auto start = getTime();

    int red_id = stoi(argv[1]);
    int black_id = stoi(argv[2]);
    SearchHelper::limit = stof(argv[3]);

#ifndef USE_IDS
    CheckersGame::SEARCH_DEPTH = stoi(argv[3]);
#endif

    ai::CheckersGame game;
    if (red_id < 0 && black_id < 0) {
        game = ai::getCheckersGame();
    } else if (black_id < 0) {
        game = ai::getNetworkVPieceCountCheckersGame(red_id, 'r');
    } else if (red_id < 0) {
        game = ai::getNetworkVPieceCountCheckersGame(black_id, 'b');
    } else {
        game = ai::getNetworkedCheckersGame(red_id, black_id);
    }

    const char winner = game.play();
    //auto end = getTime();

    if (game.moveCounter >= MOVE_LIMIT) {
        return 0;
    }

    if (winner == 'r') {
        return 1;
    } else {
        return 255;
    }
}
