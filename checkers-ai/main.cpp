#include "headers/checkers-game.h"
using ai::getCheckersGame;

#include <iostream>
using std::cout;
using std::endl;

//#include "headers/network-file-io.h"


int main() {
    auto game = getCheckersGame();
    game.play();

    cout << "gameover" << endl;
    //ai::distribution_out(100000);
    return 0;
}
