#include "../headers/network.h"
using ai::Network;
using ai::Settings::NetworkWeightType;
using ai::setupNetworks;

#include "../headers/network-file-io.h"
using ai::loadNetwork;
using ai::saveNetwork;
#include "catch.hpp"

#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
using std::ios_base;
#include <sstream>
using std::ostringstream;
#include <iostream>
using std::cout;
using std::endl;
#include <chrono>

const NetworkWeightType EPSILON = 0.00001;

double get_time() {
    return 1.0e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count();
}

bool areSame(NetworkWeightType a, NetworkWeightType b) {
    return fabs(a - b) < EPSILON;
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
        REQUIRE(areSame(player.evaluateBoard(sampleBigBoard, true), player.evaluateBoard(sampleBigBoard, true)));
        REQUIRE(areSame(player.evaluateBoard(sampleBigBoard), player.evaluateBoard(sampleBigBoard)));
    }

    SECTION ("Ensure output of a small board evaluation is consistent.") {
        REQUIRE(areSame(player.evaluateBoard(sampleSmallBoard, true), player.evaluateBoard(sampleSmallBoard, true)));
    }

    SECTION ("Ensure output of a board with twice the king weight is double") {
        player.changeKingWeight(1);
        playerAgain.changeKingWeight(2);

        REQUIRE(areSame((player.evaluateBoard(sampleSmallBoard, true) * 2), playerAgain.evaluateBoard(sampleSmallBoard, true)));
    }
    SECTION("Testing the use of the activation function is different than without") {
        REQUIRE(!areSame(player.evaluateBoard(sampleBigBoard, true), player.evaluateBoard(sampleBigBoard)));
    }
}

void writeToLogs(double timeTaken, int loopIterations, double boardsPerSec) {
    ofstream outFile;
    auto logFilePath = "logs/nn.log";
    outFile.open(logFilePath, ios_base::app);

    ostringstream outStr;
    outStr << "Timing Run:" << endl;
    outStr << "------------------------------------------------------------------" << endl;
    outStr << "Total board evaluations = " << timeTaken << " seconds" << endl;
    outStr << "Boards calculated = " << loopIterations << endl;
    outStr << "Boards evalutions per second = " << boardsPerSec << endl;
    outStr << "------------------------------------------------------------------" << endl << endl;

    outFile << outStr.str();
}

TEST_CASE("Testing the speed of board evaluation.") {
    const int LOOP_COUNTER = 10000;
    vector<unsigned int> dimesionsLarge{32, 1000, 100, 1};
    vector<unsigned int> dimesions{32, 40, 10, 1};
    setupNetworks(dimesionsLarge, 2);
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
    for (volatile int i; i < LOOP_COUNTER; ++i) {
        player.evaluateBoard(sampleBigBoard);
    }
    double evaluationEnd = get_time();
    double evaluationTotal = evaluationEnd - evaluationStart;


    double loopStart = get_time();
    for (volatile int i; i < LOOP_COUNTER; ++i) {
    }
    double loopEnd = get_time();
    double loopTotal = loopEnd - loopStart;

    double timeTaken = (evaluationTotal - loopTotal)/LOOP_COUNTER;
    auto boardsPerSec = 1 / timeTaken;
    cout << "Time taken for board evaluation was: " << timeTaken << " seconds" << endl;
    cout << "Number of boards calculated: " << LOOP_COUNTER << endl;
    cout << "Number of boards per second = " << boardsPerSec << endl;

    writeToLogs(timeTaken, LOOP_COUNTER, boardsPerSec);
}
