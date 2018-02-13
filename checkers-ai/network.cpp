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

#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::ios;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;

Network::Network(unsigned int networkId): _ID(networkId) {
	loadNetwork(_ID, *this);
}

Network::Network(
        unsigned int networkId,
        const vector<unsigned int> & layerDimensions,
        shared_ptr<Seeder> & seeder) : _ID(networkId), _performance(0), randomNumGenerator(mt19937(seeder->get())) {

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
vector<RandomNumberType> Network::getRandomNumbersOfLength(
        const unsigned int length,
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

double Network::evaluateBoard(const vector<char> & inputBoard ) {
	/*If I remember correctly, he said to just flip the sign of the final answer to get the evaluation for your opponent.
	  This evaluate function calculates for red, just flip the sign for black. */
	cout << "WARNING: Evaluator function not set!" << endl;
	//parse board
	int index = 0;
	if (DEBUG)
		cout << "This is my input board: " << endl;
	for (auto i : inputBoard) {
		if (DEBUG)
			cout << "i = " << i << endl;
		if (i == ' ' || i == 0)
			continue;
		else if (i == 'r')
			_layers[0][index] = 1;
		else if (i == 'R')
			_layers[0][index] = 1 * _kingWeight;
		else if (i == 'b')
			_layers[0][index] = -1;
		else if (i == 'B')
			_layers[0][index] = -1 * _kingWeight;
		else
			cout << "Something has slipped through!" << endl;
		++index;
	}
	// Run the board through the network. I would use range based for loop but the first vector in _layers has been filled
	for (unsigned int x = 1; x < _layers.size(); ++x) {
		if (DEBUG)
			cout << "---------------------Calculating layer: " << x << "--------------------" << endl;
		for (unsigned int y = 0; y < _layers[x].size(); ++y) {
			if(DEBUG)
				cout << "Node: " << y << " _________ ";
			calculateNode(x, y);
			// *** Insert activation function here ***
			// _layers[x][y] is the value used in the activator function
		}
	}
	// return looks funny but it will pull the last vector of the arbitrarily large _layers vector
	return _layers[_layers.size() - 1][0];
}

void Network::calculateNode(unsigned int x, unsigned int y) {
	_layers[x][y] = 0;
	unsigned int previousLayerSize = _layers[x - 1].size();

	for (unsigned int i = 0; i < previousLayerSize; ++i) {
		_layers[x][y] += _weights[x][y*previousLayerSize + i] * _layers[x - 1][i];
	}

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

vector<Network::NetworkWeights> Network::evolve() const { 	// *** TODO *** Not required for Project 2
    vector<NetworkWeights> dummy;
    return dummy;
}

void Network::replaceWithEvolution(const Network & inputNetwork) {
    _weights = std::move(inputNetwork.evolve());
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

void Network::changeKingWeight(double newWeight) {
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
	if (lhs._ID != rhs._ID)
		return false;
	if (lhs._kingWeight != rhs. _kingWeight)
		return false;
	if (lhs._performance != rhs._performance)
		return false;
	if (lhs._layers != rhs._layers)
		return false;
	if (lhs._weights != rhs._weights)
		return false;

	return true;
}

void ai::setupNetworks(const vector<unsigned int>& dimensions, int numberOfNetworks) { //numberOfNetworks = 100
    std::cout << "You are about to setup a new set of networks. This operation will overwrite previous networks. \n" <<
        "Are you sure you want to continue? (y,n) ";
    if (std::cin.get() == 'n') {
        cout << "Not overwriting files" << endl;
        return;
    }

    auto seeder = getSeeder();
    for (auto index = 0; index < numberOfNetworks; ++index) {
        Network(index, dimensions, seeder);
    }
}
