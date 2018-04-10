#include "../headers/network.h"
using ai::Network;
using ai::Settings::NetworkWeightType;
using ai::setupNetworks;

#include "../headers/utils.h"
using ai::getTime;

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

#include "../headers/consts.h"

ai::NetworkTestingNamechange t;

TEST_CASE("Testing setupNetworks") {
    vector<unsigned int> dims = {32, 1000, 100, 1, 32, 1000, 100, 1, 32, 1000, 100, 1};
    setupNetworks(dims, 2);
}

TEST_CASE("Test saving and loading consistency") {
    ai::Network player(0);
    ai::Network playerAgain(0);

    SECTION ("Testing loading consistency") {
        REQUIRE(player == playerAgain);
    }

    SECTION ("Loading a saved object == object that was saved.") {
        Network::save(0, player);
        Network::load(0, playerAgain);

        REQUIRE(player == playerAgain);
    }

    SECTION ("Loading an unavailable network fails") {
        ai::Network failLoadTest(0);

        REQUIRE_FALSE(Network::load(101, failLoadTest));
    }
}

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
vector<char> sampleSmallBoard{
        'R',   ' ',   ' ',   ' ',
    ' ',   ' ',   ' ',   ' ',
        ' ',   ' ',   ' ',   ' ',
    ' ',   ' ',   ' ',   ' ',
        ' ',   ' ',   ' ',   ' ',
    ' ',   ' ',   ' ',   ' ',
        ' ',   ' ',   ' ',   ' ',
    ' ',   ' ',   ' ',   ' '
    };

TEST_CASE("Test Network Evaluation", "[network]") {
    ai::Network player(0);
    ai::Network playerAgain(0);

    SECTION("test network evaluation of empty board is 0.") {
        REQUIRE(player.evaluateBoard(emptyBoard) == Approx(0));
        REQUIRE(player.evaluateBoard(emptyBoard, true) == Approx(0));
    }

    SECTION ("Ensure output of a big board evaluation is consistent.") {
        SECTION("testing == true") {
            auto run1 = player.evaluateBoard(sampleBigBoard, true);
            auto run2 = player.evaluateBoard(sampleBigBoard, true);

            REQUIRE(run1 - run2 == Approx(0));
        }

        SECTION("testing == false") {
            auto run1 = player.evaluateBoard(sampleBigBoard);
            auto run2 = player.evaluateBoard(sampleBigBoard);

            REQUIRE(run1 - run2 == Approx(0));
        }
    }

    SECTION ("Ensure output from a board with a single input node works and is consistent.") {
        for (int index = 0; index < 32; index++) {
            vector<char> sampleLoopBoard(32);
            sampleLoopBoard[index] = 'R';

            REQUIRE(player.evaluateBoard(sampleLoopBoard) != Approx(0));

            auto run1 = player.evaluateBoard(sampleLoopBoard, true);
            auto run2 = player.evaluateBoard(sampleLoopBoard, true);

            REQUIRE(run1 - run2 == Approx(0));
        }
    }

    SECTION ("Ensure output of evaluation given different boards is different.") {
        NetworkWeightType big = 0, empty = 0, small = 0;

        SECTION("testing == true") {
            big = player.evaluateBoard(sampleBigBoard, true);
            empty = player.evaluateBoard(emptyBoard, true);
            small = player.evaluateBoard(sampleSmallBoard, true);
        }

        SECTION("testing == false") {
            big = player.evaluateBoard(sampleBigBoard);
            empty = player.evaluateBoard(emptyBoard);
            small = player.evaluateBoard(sampleSmallBoard);
        }

        REQUIRE(big - empty != Approx(0));
        REQUIRE(small - empty != Approx(0));
        REQUIRE(big - small != Approx(0));
    }

    SECTION ("Ensure output of a board with twice the king weight is double") {
        player.changeKingWeight(1);
        playerAgain.changeKingWeight(2);

        auto withKingWeightOf1 = player.evaluateBoard(sampleSmallBoard, true);
        auto withKingWeightOf2 = playerAgain.evaluateBoard(sampleSmallBoard, true);

        REQUIRE(withKingWeightOf1 * 2 - withKingWeightOf2 == Approx(0));
    }

    SECTION("Testing the use of the activation function is different than without") {
        auto withActivation =player.evaluateBoard(sampleBigBoard, true );
        auto withOutActivation = player.evaluateBoard(sampleBigBoard);

        REQUIRE(withActivation - withOutActivation != Approx(0));
    }

    SECTION("Testing evaluation of 0-24 piece boards.") {
        vector<char> board{
                'r',   'r',   'r',   'r',
            'r',   'r',   'r',   'r',
                'r',   'r',   'r',   'r',
            'b',   'b',   'b',   'b',
                'b',   'b',   'b',   'b',
            'b',   'b',   'b',   'b',
                ' ',   ' ',   ' ',   ' ',
            ' ',   ' ',   ' ',   ' '    };
            
        Network player(0);
        for (auto i = 0; i <= 24; ++i) {
                player.evaluateBoard(board);
                board[i] = ' ';
            }
    }
}

TEST_CASE("Testing evolution") {
    ai::Network player(0);
    ai::Network playerZeroEvolved(1);

    playerZeroEvolved.evolveUsingNetwork(player);
    // player.outputCreationDebug();
    // playerZeroEvolved.outputCreationDebug();
    REQUIRE(nothingSimilar(player, playerZeroEvolved));
}

void writeToLogs(double timeTaken, int loopIterations, double boardsPerSec) {
    ofstream outFile;
    auto logFilePath = "logs/nn.log";
    outFile.open(logFilePath, ios_base::app);

    ostringstream outStr;
    outStr << "Timing Network Eval:" << endl;
    outStr << "------------------------------------------------------------------" << endl;
    outStr << "Total board evaluations = " << timeTaken << " seconds" << endl;
    outStr << "Boards calculated = " << loopIterations << endl;
    outStr << "Boards evalutions per second = " << boardsPerSec << endl;
    outStr << "------------------------------------------------------------------" << endl << endl;

    outFile << outStr.str();
}

TEST_CASE("Testing the speed of board evaluation.", "[network-timing]") {
    const int LOOP_COUNTER = 1000;
    vector<unsigned int> dims_over10k{32, 1000, 100, 1};
    // Recommended choices are ai::blondieDimensions and dims_over10k
    setupNetworks(ai::blondieDimensions, 2);
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
    cout << "\n\n\n\n****** Getting average board evaluation time.. This could take a while ******" << endl;
    double averageTime = 0;
    const unsigned int LOOPSFORAVERAGE = 1000;
    for (volatile unsigned int index = 0; index < LOOPSFORAVERAGE; ++index){
        double evaluationStart = getTime();
        for (volatile int i = 0; i < LOOP_COUNTER; ++i) {
            player.evaluateBoard(sampleBigBoard);
        }
        double evaluationEnd = getTime();
        double evaluationTotal = evaluationEnd - evaluationStart;


        double loopStart = getTime();
        for (volatile int j = 0; j < LOOP_COUNTER; ++j) {
        }
        double loopEnd = getTime();
        double loopTotal = loopEnd - loopStart;

        averageTime += (evaluationTotal - loopTotal)/LOOP_COUNTER;
    }

    averageTime /= LOOPSFORAVERAGE;
    double averageBPS = 1/averageTime;


    cout << endl << "Network Evaluation Timing: " << endl;
    cout << "----------------------------------" << endl;
    cout << "Average time taken for board evaluation was: " << averageTime << " seconds" << endl;
    cout << "Number of boards calculated: " << LOOP_COUNTER * LOOPSFORAVERAGE << endl;
    cout << "Number of boards per second = " << averageBPS << endl;

    writeToLogs(averageTime, LOOP_COUNTER, averageBPS);
}
