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
    //std::mt19937 rander;
    //ai::distribution_out(rander);
    return 0;
}
