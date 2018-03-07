#include "headers/checkers-game.h"
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "headers/network.h"
using ai::Network;
using ai::weightChangeOut;

#include <iostream>
using std::cout;
using std::endl;




int main(int argc, char *argv[]) {
    unsigned int red_id = *argv[1];
    unsigned int black_id = *argv[2];
    auto game = getNetworkedCheckersGame(red_id, black_id);
    game.play();

    Network net1(0), net2(1);
    net2.evolveUsingNetwork(net1);
    weightChangeOut(net1,net2);

    cout << "gameover" << endl;
    return 0;
}
