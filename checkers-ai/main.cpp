#include "headers/checkers-game.h"
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "headers/network.h"
using ai::Network;
using ai::weightChangeOut;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;
using std::stoi;




int main(int argc, char *argv[]) {
    unsigned int red_id = stoi(string(1, *argv[1]));
    unsigned int black_id = stoi(string(1,*argv[2]));
    cout<<"got id's"<<endl;
    cout<<"id 1 is "<<red_id<<endl;
    auto game = getNetworkedCheckersGame(red_id, black_id);
    game.play();

    Network net1(0), net2(1);
    net2.evolveUsingNetwork(net1);
    weightChangeOut(net1,net2);

    cout << "gameover" << endl;
    return 0;
}
