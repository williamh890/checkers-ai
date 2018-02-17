#include "headers/seeder.h"
using ai::getSeeder;
using ai::Seeder;

#include "headers/network.h"
using ai::Network;

#include "headers/network-file-io.h"
using ai::loadNetwork;
using ai::saveNetwork;

#include "headers/consts.h"
using ai::DEBUG;
using NetworkWeightType = ai::Settings::NetworkWeightType;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <random>
using std::mt19937;
using std::uniform_real_distribution;

#include <memory>
using std::shared_ptr;

Network::Network(unsigned int networkId): _ID(networkId) {
	loadNetwork(_ID, *this);
}

Network::Network(unsigned int networkId,
                const vector<unsigned int> & layerDimensions,
                shared_ptr<Seeder> & seeder) :  _ID(networkId), 
                                                _performance(0), 
                                                randomNumGenerator(mt19937(seeder->get())) {

    setupLayers(layerDimensions);
    setupRandomWeights(layerDimensions);
    setupKingWeight();

    saveNetwork(_ID, *this);
}

void Network::setupLayers(const vector<unsigned int> & layerDimensions) {
    for (auto & layerDimension : layerDimensions) {
        vector<NetworkWeightType> layer(layerDimension, 0);

        _layers.push_back(layer);
    }
}

void Network::setupRandomWeights(const vector<unsigned int> & layerDimensions){
    auto firstLayerSize = layerDimensions.at(0);

    uniform_real_distribution<NetworkWeightType> distribution(-1, 1);
    auto firstLayerWeights = getRandomNumbersOfLength(firstLayerSize, distribution);
    _weights.push_back(firstLayerWeights);

    for (auto i = 0; (unsigned int)i < layerDimensions.size() - 1; ++i) {
        auto currentLayerDim = layerDimensions.at(i);
        auto nextLayerDim = layerDimensions.at(i + 1);

        auto totalLayerWeights = currentLayerDim * nextLayerDim;
        auto weights = getRandomNumbersOfLength(totalLayerWeights, distribution);

        _weights.push_back(weights);
    }
}

void Network::setupKingWeight() {
    _kingWeight = 2;
}

template <typename RandomNumberType>
vector<RandomNumberType> Network::getRandomNumbersOfLength( const unsigned int length,
                                                            uniform_real_distribution<RandomNumberType> & distribution) {
    vector<RandomNumberType> rngNumbers;
    for (auto i = 0; (unsigned int)i < length; ++i) {
        auto randomNumber = distribution(randomNumGenerator);

        rngNumbers.push_back(randomNumber);
    }
    return rngNumbers;
}

Network::~Network() {
    if (_gameCompleted) {
        saveNetwork(_ID, *this);
    }
}

// evaluateBoard returns an answer for red. Flip the sign for black.
NetworkWeightType Network::evaluateBoard(const vector<char> & inputBoard, bool testing) { // testing defaults false
    /*void inputBoardIntoFirstLayer(inputBoard)*/ {
        int index = 0;
        for (const auto & i : inputBoard) {
            //if (DEBUG)
            //	cout << "i = " << i << endl;

            if (i == ' ' || i == 0)
                _layers[0][index] = 0;
            else if (i == 'r')
                _layers[0][index] = 1;
            else if (i == 'R')
                _layers[0][index] = 1 * _kingWeight;
            else if (i == 'b')
                _layers[0][index] = -1;
            else if (i == 'B')
                _layers[0][index] = -1 * _kingWeight;
            else
                cout << "Unrecognized character in board: " << i << endl;
            ++index;
        }
    }
    /*void feedForward()*/{
        for (unsigned int x = 1; x < _layers.size(); ++x) {
            # pragma omp parallel for schedule(guided, 2) firstprivate(x, testing) default(none)
            for (unsigned int y = 0; y < _layers[x].size(); ++y) {
                /*calculateNode(x, y)*/ {
                    NetworkWeightType total1 = 0, total2 = 0, total3 = 0, total4 = 0;
                    unsigned int previousLayerSize = _layers[x - 1].size();

                    for (unsigned int i = 0; i < previousLayerSize; i+=4) {
                        total1 += _weights[x][y*previousLayerSize + i] * _layers[x - 1][i];
                        total2 += _weights[x][y*previousLayerSize + i + 1] * _layers[x - 1][i + 1];
                        total3 += _weights[x][y*previousLayerSize + i + 2] * _layers[x - 1][i + 2];
                        total4 += _weights[x][y*previousLayerSize + i + 3] * _layers[x - 1][i + 3];
                    }

                    _layers[x][y] = total1 + total2 + total3 + total4;
                }
                if (testing) {
                    continue;
                }
                /*useActivationFunction*/ {
                    NetworkWeightType var = _layers[x][y];
                    _layers[x][y] = var / (1 + abs(var));
                }
            }
        }
    }
	return _layers[_layers.size() - 1][0] /*boardEvaluationOutput()*/;
}

inline NetworkWeightType Network::activationFunction(NetworkWeightType x) {
    // Fast Sigmoid
    return x / (1 + abs(x));
}


void Network::calculateNode(unsigned int x, unsigned int y) {
    NetworkWeightType totalNodeValue = 0;
    unsigned int previousLayerSize = _layers[x - 1].size();

	for (unsigned int i = 0; i < previousLayerSize; i+=2) {
		totalNodeValue += _weights[x][y*previousLayerSize + i] * _layers[x - 1][i];
	}

	_layers[x][y] = totalNodeValue;

	if (DEBUG)
		cout << _layers[x][y] << endl;
}

void Network::adjustPerformance(int resultFromGame) {
    _performance += resultFromGame;
    _gameCompleted = true;
}

int Network::getPerformance() const {
    return _performance;
}

void Network::resetPerformance() {
    _performance = 0;
}

vector<Network::NetworkWeights> Network::evolve() const { 	// *** TODO *** Required for Project 3
    vector<NetworkWeights> weights_to_pass = _weights;
    NetworkWeightType tempKing = _kingWeight;
    // *** Evolve tempKing
    // *** Evolve sigma
    // *** Evolve weights in weights_to_pass
    
    weights_to_pass.push_back(vector<NetworkWeightType>(tempKing)); //Add tempking to the back of the weights vector for passing
    return weights_to_pass;
}

void Network::replaceWithEvolution(vector<NetworkWeights> & inputWeights) {
    _kingWeight = inputWeights[inputWeights.size()-1][0];
    inputWeights.pop_back();
    _weights = std::move(inputWeights);
    saveNetwork (_ID, *this);
}

void Network::outputCreationDebug() {
    cout << "Weight for the king: " << _kingWeight << endl;
    cout << "Number of layers: " << _layers.size() << endl;

    for (unsigned int index = 0; index < _layers.size(); ++index) {
        cout << "Size of layer " << index << " = " << _layers[index].size() << endl;
    }

    cout << "size of weights vector: " << _weights.size() << endl;
    for (unsigned int index = 0; index < _weights.size(); ++index) {
        cout << "Size of weight layer " << index << " = " << _weights[index].size() << endl;
    }

    cout << "_weights data: " << endl;
    for (auto & v : _weights) {
        for (auto x : v) {
            cout << x << " ";
        }
        cout << endl;
    }
}

void Network::changeKingWeight(NetworkWeightType newWeight) {
	_kingWeight = newWeight;
}

bool ai::operator<(const Network & lhs, const Network & rhs) {
    return lhs.getPerformance() < rhs.getPerformance();
}

bool ai::operator>(const Network & lhs, const Network & rhs) {
    return rhs < lhs;
}

bool ai::operator<=(const Network & lhs, const Network & rhs) {
    return !(lhs > rhs);
}

bool ai::operator>=(const Network & lhs, const Network & rhs) {
    return !(lhs < rhs);
}

bool ai::operator== (const Network &lhs, const Network &rhs) {
    return
        (lhs._ID == rhs._ID) and
        (lhs._kingWeight == rhs. _kingWeight) and
        (lhs._performance == rhs._performance) and
        (lhs._layers == rhs._layers) and
        (lhs._weights == rhs._weights);
}

void ai::setupNetworks(const vector<unsigned int>& dimensions, int numberOfNetworks) { //numberOfNetworks = 100
    std::cout << "You are about to setup a new set of networks. This operation will overwrite previous networks. \n" <<
        "Are you sure you want to continue? (y,n) ";
    if (std::cin.get() == 'n') {
        cout << "Not overwriting files" << endl;
        std::cin.ignore();
        return;
    }

    auto seeder = getSeeder();
    for (auto index = 0; index < numberOfNetworks; ++index) {
        Network(index, dimensions, seeder);
    }
    std::cin.ignore();
}
