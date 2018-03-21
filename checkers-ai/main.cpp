#include "headers/checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "headers/network.h"
using ai::Network;
using ai::weightChangeOut;

#include "headers/consts.h"
using ai::MOVE_LIMIT;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;
using std::stoi;

int main(int argc, char** argv) {
    unsigned int red_id = stoi(argv[1]);
    unsigned int black_id = stoi(argv[2]);

    auto game = getNetworkedCheckersGame(red_id, black_id);
    const char winner = game.play();
    cout << "gameover" << endl;
    cout<<"winner was "<<winner<<endl;

    if (game.moveCounter >= MOVE_LIMIT) {
        return 0;
    }

    if (winner == 'r'){
        return 1;
    } else  {
        return 255;
    }
}
