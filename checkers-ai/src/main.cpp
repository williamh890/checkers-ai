#include "checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "network.h"
using ai::Network;
using ai::weightChangeOut;

#include "utils.h"
using ai::getTime;

#include "consts.h"
using ai::MOVE_LIMIT;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::stoi;
using std::string;

int main(int argc, char** argv) {
    auto start = getTime();

    unsigned int red_id = stoi(argv[1]);
    unsigned int black_id = stoi(argv[2]);

    auto game = getNetworkedCheckersGame(red_id, black_id);

    const char winner = game.play();

    auto end = getTime();

    cout << "game: " << end - start << endl;
    if (game.moveCounter >= MOVE_LIMIT) {
        return 0;
    }

    if (winner == 'r'){
        return 1;
    } else  {
        return 255;
    }
}
