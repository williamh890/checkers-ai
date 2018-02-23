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
using std::normal_distribution;

#include <memory>
using std::shared_ptr;

#include <math.h>

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
    setupSigmas();

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

    uniform_real_distribution<NetworkWeightType> distribution(-0.2, 0.2);
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
    uniform_real_distribution<NetworkWeightType> distribution(1,3);
    _kingWeight = distribution(randomNumGenerator);
}

void Network::setupSigmas() {
    _sigmas = _weights;
    for(unsigned int i = 0; i < _sigmas.size(); ++ i) {
        for (unsigned int ii = 0; ii < _sigmas[i].size(); ++ii) {
            _sigmas[i][ii] = .05;
        }
    }
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

                    auto total = total1 + total2 + total3 + total4;

                    _layers[x][y] = (!testing) ? total / (1 + abs(total)) : total;
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

void Network::evolve() { 	// *** TODO *** Required for Project 3
    evolveKingWeight();
    evolveSigmas();
    evolveWeights();    
}

void Network::evolveKingWeight() {
    uniform_real_distribution<NetworkWeightType> distribution(-1,1);
    _kingWeight += distribution(randomNumGenerator);
}

NetworkWeightType Network::getTau() {
    NetworkWeightType numberofWeights = 0;
    for (unsigned int index = 0; index < _weights.size(); ++index) {
        numberofWeights += _weights[index].size();
    }
    NetworkWeightType tau = 1/(sqrt(2*sqrt(numberofWeights)));
    return tau;
}
void Network::evolveSigmas() {
    auto tau = getTau();
    for (unsigned int i = 0; i < _sigmas.size(); ++i) {
        for (unsigned int ii = 0; ii < _sigmas[i].size(); ++ii) {
            _sigmas[i][ii] = _sigmas[i][ii] * exp(tau * gaussianNumbersZeroToOne(randomNumGenerator));
        }
    }
}

void Network::evolveWeights() {
    for (unsigned int i = 0; i < _weights.size(); ++i) {
            for (unsigned int ii = 0; ii < _weights[i].size(); ++ii) {
                _weights[i][ii] = _weights[i][ii] + _sigmas[i][ii] * gaussianNumbersZeroToOne(randomNumGenerator);
            }
        }
}
void Network::replaceWithEvolution(const Network & rhs) {
    _kingWeight = rhs._kingWeight;
    _weights = rhs._weights;
    _sigmas = rhs._sigmas;
    this->evolve();

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

    cout << "_sigmas data: " << endl;
    for (auto & v : _sigmas) {
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

NetworkWeightType ai::gaussianNumbersZeroToOne(std::mt19937 & randomNumGenerator) { // TODO: Make this return a Gaussian number
    normal_distribution<NetworkWeightType> distribution(0, 1);
    return distribution (randomNumGenerator);
}
