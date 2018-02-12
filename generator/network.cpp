#include "headers/seeder.h"
using ai::getSeeder;
using ai::Seeder;

#include "headers/consts.h"
using ai::DEBUG;
#include "headers/network.h"
using ai::Network;

#include "headers/network-file-io.h"
using ai::loadNetwork;
using ai::saveNetwork;

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


Network::Network(unsigned int inputID): _ID(inputID) {
	loadNetwork(_ID, *this);
};

Network::Network(
        const vector<unsigned int> & inputdimensions,
        unsigned int inputID,
        shared_ptr<Seeder> & seeder) : _ID(inputID), _performance(0) {

	_layers.resize(inputdimensions.at(0));
	// Sizing each layer containing nodes within the _layers vector
	if (DEBUG)
    	cout << "inputdimensions: ";
	for (unsigned int index = 1; index <= inputdimensions.at(0); ++index) {
		_layers[index-1].resize(inputdimensions.at(index));
		if (DEBUG)
        	cout << inputdimensions.at(index) << " ";
	} 
	if (DEBUG)
		cout << endl;

	_weights.resize(inputdimensions.at(0));
	// Sizing each network weights vector contained in _weights
	_weights[0].resize(inputdimensions.at(1));
	for (unsigned int i = 1; i < _weights.size(); ++i) {
		_weights[i].resize(inputdimensions.at(i) * inputdimensions.at(i + 1));
	}

	mt19937 randomNumGenerator(seeder->get());
	uniform_real_distribution<double> distribution(-1, 1);

	_weights.resize(inputdimensions.at(0));
	_weights[0].resize(inputdimensions.at(1));

	for (auto i = 1; (unsigned int)i < _weights.size(); ++i) {
		_weights[i].resize(inputdimensions.at(i) * inputdimensions.at(i + 1));
	}

	for (auto i = 0; (unsigned int)i < _weights.size(); i++) {
		for (auto j = 0; (unsigned int)j < _weights[i].size(); j++) {
			_weights[i][j] = distribution(randomNumGenerator);
		}
	}

	// fill the _weights vector of networkWeights with random values -1 < x < 1
	uniform_real_distribution<double> kingWeightRange(0, 5);
	//_kingWeight = kingWeightRange(randomNumGenerator);
	_kingWeight = 2;
	saveNetwork(_ID, *this);
}

Network::~Network() {
	if (_gameCompleted) {
		//saveNetwork(_ID, *this);
	}
};

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
				cout << "Node: " << y << endl;
			calculateNode(x, y);
			// *** Insert activation function here ***
			// _layers[x][y] is the value used in the activator function
		}
	}
	// return looks funny but it will pull the last vector of the arbitrarily large _layers vector
	return _layers[_layers.size() - 1][0];
};

void Network::calculateNode(unsigned int x, unsigned int y) {
	unsigned int previousLayerSize = _layers[x - 1].size();
	for (unsigned int i = 0; i < previousLayerSize; ++i) {
		_layers[x][y] += _weights[x][y*previousLayerSize + i] * _layers[x - 1][i];
	}
}
void Network::adjustPerformance(int resultFromGame) {
	_performance += resultFromGame;
	_gameCompleted = true;
};

int Network::getPerformance() const {
	return _performance;
}
void Network::resetPerformance() {
	_performance = 0;
};
vector<Network::networkWeights> Network::evolve() const { 	// *** TODO *** Not required for Project 2
	vector<networkWeights> dummy;
	return dummy;
};
void Network::replaceWithEvolution(const Network & inputNetwork) {
	_weights = std::move(inputNetwork.evolve());
};

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

bool ai::operator<(const Network & lhs, const Network & rhs) {
    return lhs.getPerformance() < rhs.getPerformance();
}

bool ai::operator>(const Network & lhs, const Network & rhs) {return rhs < lhs;}
bool ai::operator<=(const Network & lhs, const Network & rhs) {return !(lhs > rhs);}
bool ai::operator>=(const Network & lhs, const Network & rhs) {return !(lhs < rhs);}

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
	if (std::cin.get() == 'n')
		return;

    auto seeder = getSeeder();
	for (auto index = 0; index < numberOfNetworks; ++index) {
		Network(dimensions, index, seeder);
	}
};


