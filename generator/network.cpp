#include "headers\network.h"
using AI::Network;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

#include "headers\consts.h"
#include <random>
using std::mt19937;
using std::uniform_real_distribution;

#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;
#include <boost\serialization\vector.hpp>
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
#include <string>
using std::string;
using std::to_string;



Network::Network(unsigned int inputID): _ID(inputID) {
	loadNetwork(_ID, *this);
};

Network::Network(const vector<unsigned int> & inputdimensions, unsigned int inputID) : _ID(inputID), _performance(0) {
	_layers.resize(inputdimensions.at(0));
	for (unsigned int index = 1; index <= inputdimensions.at(0); ++index) {
		_layers[index-1].resize(inputdimensions.at(index));
	}

	std::random_device device;
	mt19937 randomNumGenerator;
	if (ai::Settings::SEEDING_METHOD == "random_device") {
		randomNumGenerator = mt19937(device());
	}
	if (ai::Settings::SEEDING_METHOD == "time") {
		srand(time(NULL));
		randomNumGenerator = mt19937(rand());
	}
	uniform_real_distribution<double> distribution(-1, 1);

	_weights.resize(inputdimensions.at(0));
	_weights[0].resize(inputdimensions.at(1));
	for (unsigned int i = 1; i < _weights.size(); ++i) {
		_weights[i].resize(inputdimensions.at(i) * inputdimensions.at(i + 1));
	}
	for (std::vector<std::vector<int>>::size_type i = 0; i < _weights.size(); i++) {
		for (std::vector<int>::size_type j = 0; j < _weights[i].size(); j++) {
			_weights[i][j] = distribution(randomNumGenerator);
		}
	}
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

bool AI::operator<(const Network & lhs, const Network & rhs) {
	if (lhs.getPerformance() < rhs.getPerformance())
		return true;
	return false;
}

bool AI::operator>(const Network & lhs, const Network & rhs) {return rhs < lhs;}
bool AI::operator<=(const Network & lhs, const Network & rhs) {return !(lhs > rhs);}
bool AI::operator>=(const Network & lhs, const Network & rhs) {return !(lhs < rhs);}

void AI::setupNetworks(const vector<unsigned int>& dimensions, int numberOfNetworks) { //numberOfNetworks = 100
	std::cout << "You are about to setup a new set of networks. This operation will overwrite previous networks. \n" <<
		"Are you sure you want to continue? (y,n) ";
	if (std::cin.get() == 'n')
		return;

	for (auto index = 0; index < numberOfNetworks; ++index) {
		Network(dimensions, index);
	}
};

string AI::idToFilename(int ID) {
	string filename = to_string(ID) + ".network";
	// The following implementation will be used once we begin to get the network integrated.
	//string filename = ".\\networks\\" + to_string(ID) + ".network"; //creates filenames that scope to a folder called networks
	return filename;
}

void AI::saveNetwork(int ID, Network & networkToSave) {
	ofstream file{ idToFilename(ID) };
	text_oarchive oa{ file };
	oa << networkToSave;
}

bool AI::loadNetwork(int ID, Network & networkRecievingData) {
	ifstream file{ idToFilename(ID) };
	if (!file) {
		std::cout << "The file does not exist" << std::endl;
		return false;
	}
	text_iarchive ia{ file };
	ia >> networkRecievingData;
	return true;
}