#include "headers/checkers-game.h"
using ai::getCheckersGame;
using ai::getNetworkedCheckersGame;

#include "headers/network.h"
using ai::Network;
using ai::weightChangeOut;

#include "headers/minimax.h"
using ai::MiniMaxHelper;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;
using std::stoi;

int main(int argc, char** argv) {
    unsigned int red_id = stoi(argv[1]);
    unsigned int black_id = stoi(argv[2]);
    //cout<<"got id's"<<endl;
    //cout<<"red id is "<<red_id<<endl;
    //cout<<"black id is "<<black_id<<endl;
    auto game = getNetworkedCheckersGame(red_id, black_id);
    const char winner = game.play();
    cout << "gameover" << endl;
    cout<<"winner was "<<winner<<endl;
    //auto h = MiniMaxHelper('b', game);
    //float out = game.activePlayer->baseCase(h);
    //cout << "bef output: " << out << endl;

    //if (winner == 'r'){
      //return 1;
    //}

    //Network net1(0), net2(1);
    //net2.evolveUsingNetwork(net1);
    //weightChangeOut(net1,net2);


    return 0;
}
