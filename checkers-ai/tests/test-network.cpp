#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "../headers/network-file-io.h"
using ai::loadNetwork;
using ai::saveNetwork;
#include "catch.hpp"

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <chrono>

double get_time() {
    return 1.0e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count();
}


TEST_CASE("Testing setupNetworks") {
    vector<unsigned int> dimesions{32, 40, 10, 1};
    setupNetworks(dimesions, 2);
}

TEST_CASE("Test saving and loading consistency") {
    ai::Network player(0);
    ai::Network playerAgain(0);

    SECTION ("Testing loading consistency") {
        REQUIRE(player == playerAgain);
    }

    SECTION ("Loading a saved object == object that was saved.") {
        saveNetwork(0, player);
        loadNetwork(0, playerAgain);

        REQUIRE(player == playerAgain);
    }

    SECTION ("Loading an unavailable network fails") {
        ai::Network failLoadTest(0);

        REQUIRE(loadNetwork(101, failLoadTest) == false);
    }
}

TEST_CASE("Test Network Evaluation") {
    ai::Network player(0);
    ai::Network playerAgain(0);

    vector<char> emptyBoard(32);
    vector<char> sampleBigBoard{
            'r',   'r',   'r',   'r',
         'r',   'r',   'r',   'r',
            ' ',   'r',   'r',   'r',
         ' ',   ' ',   'r',   ' ',
            ' ',   ' ',   'b',   ' ',
         'b',   'b',   ' ',   'b',
            'b',   'b',   'b',   'b',
         'b',   'b',   'b',   'b'
        };
        vector<char> sampleSmallBoard(32);
        sampleSmallBoard[0] = 'R';

    SECTION("test network evaluation of empty board is 0.") {
        REQUIRE(player.evaluateBoard(emptyBoard) == 0);
        REQUIRE(player.evaluateBoard(emptyBoard, true) == 0);
    }

    SECTION ("Ensure output of a big board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleBigBoard, true) == player.evaluateBoard(sampleBigBoard, true));
        REQUIRE(player.evaluateBoard(sampleBigBoard) == player.evaluateBoard(sampleBigBoard));
    }

    SECTION ("Ensure output of a small board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleSmallBoard, true) == player.evaluateBoard(sampleSmallBoard, true));
    }

    SECTION ("Ensure output of a board with twice the king weight is double") {
        player.changeKingWeight(1);
        playerAgain.changeKingWeight(2);

        REQUIRE((player.evaluateBoard(sampleSmallBoard, true) * 2) == playerAgain.evaluateBoard(sampleSmallBoard, true));
    }
    SECTION("Testing the use of the activation function is different than without") {
        REQUIRE(player.evaluateBoard(sampleBigBoard, true) != player.evaluateBoard(sampleBigBoard));
    }
}


TEST_CASE("Testing the speed of board evaluation.") {
    int LOOPCOUNTER = 10000;
    vector<unsigned int> dimesionsLarge{32, 1000, 100, 1};
    vector<unsigned int> dimesions{32, 40, 10, 1};
    setupNetworks(dimesions, 2);
    vector<char> sampleBigBoard{
            'r',   'r',   'r',   'r',
         'r',   'r',   'r',   'r',
            ' ',   'r',   'r',   'r',
         ' ',   ' ',   'r',   ' ',
            ' ',   ' ',   'b',   ' ',
         'b',   'b',   ' ',   'b',
            'b',   'b',   'b',   'b',
         'b',   'b',   'b',   'b'
        };
    Network player(0);

    cout << "\n\n\n\n****** Getting board evaluation time.. This could take a while ******" << endl;

    double evaluationStart = get_time();
    for (volatile int i; i < LOOPCOUNTER; ++i) {
        player.evaluateBoard(sampleBigBoard);
    }
    double evaluationEnd = get_time();
    double evaluationTotal = evaluationEnd - evaluationStart;


    double loopStart = get_time();
    for (volatile int i; i < LOOPCOUNTER; ++i) {
    }
    double loopEnd = get_time();
    double loopTotal = loopEnd - loopStart;


    double timeTaken = (evaluationTotal - loopTotal)/LOOPCOUNTER;
    cout << "Time taken for board evaluation was: " << timeTaken << " seconds" << endl;
    cout << "Number of boards calculated: " << LOOPCOUNTER << endl;
    cout << "Number of boards per second = " << 1/timeTaken << endl;
}
