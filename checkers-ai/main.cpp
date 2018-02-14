#include "headers/checkers-game.h"
using ai::getCheckersGame;

#include <iostream>
using std::cout;
using std::endl;


int main() {
    auto game = getCheckersGame();
    game.play();

    cout << "gameover" << endl;
    return 0;
}
