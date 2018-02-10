#include "headers/seeder.h"
using ai::getSeeder;
using ai::Seeder;

#include "headers/network.h"
using ai::Network;

#include "headers/consts.h"

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
#include <string>
using std::string;
using std::to_string;
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
	for (unsigned int index = 1; index <= inputdimensions.at(0); ++index) {
		_layers[index-1].resize(inputdimensions.at(index));
	}

	mt19937 randomNumGenerator(seeder->get());
	uniform_real_distribution<double> distribution(-1, 1);

	_weights.resize(inputdimensions.at(0));
	_weights[0].resize(inputdimensions.at(1));

	for (auto i = 1; (unsigned int)i < _weights.size(); ++i) {
		_weights[i].resize(inputdimensions.at(i) * inputdimensions.at(i + 1));
	}
	for (auto i = 0; i < _weights.size(); i++) {
		for (auto j = 0; j < _weights[i].size(); j++) {
			_weights[i][j] = distribution(randomNumGenerator);
		}
	}

	// fill the _weights vector of networkWeights with random values -1 < x < 1
	saveNetwork(_ID, *this);
}

Network::~Network() {
	if (_gameCompleted) {
		saveNetwork(_ID, *this);
	}
};

double Network::evaluateBoard(const vector<char> & inputBoard ) const { 	//***TODO***
	double dummy = 0;
	return dummy;
};
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

void Network::outputDebug() {
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

string ai::idToFilename(int ID) {
	string filename = to_string(ID) + ".network";
	// The following implementation will be used once we begin to get the network integrated.
	//string filename = ".\\networks\\" + to_string(ID) + ".network"; //creates filenames that scope to a folder called networks
	return filename;
}

void ai::saveNetwork(int ID, Network & networkToSave) {
}

bool ai::loadNetwork(int ID, Network & networkRecievingData) {
    return true;
}
