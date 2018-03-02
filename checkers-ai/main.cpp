#include "headers/checkers-game.h"
using ai::getCheckersGame;

#include <iostream>
using std::cout;
using std::endl;

#include "headers/network.h"
using ai::Network;
using ai::weightChangeOut;


int main() {
    auto game = getCheckersGame();
    game.play();

    Network net1(0), net2(1);
    net2.evolveUsingNetwork(net1);
    weightChangeOut(net1,net2);

    cout << "gameover" << endl;
    return 0;
}
