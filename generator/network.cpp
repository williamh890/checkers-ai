#include "headers\network.h"
using AI::Network;
#include <vector>
using std::vector;

#include "boost\archive\text_iarchive.hpp"
#include "boost\archive\text_oarchive.hpp"
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;
#include "boost\serialization\vector.hpp"
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
#include <string>
using std::string;
using std::to_string;



Network::Network(int inputID): _ID(inputID) {
	loadNetwork(_ID, *this);
};

Network::Network(const vector<int> & inputdimensions) {
	setupNetwork(inputdimensions);
}

Network::~Network() {
	if (_gameCompleted) {
		saveNetwork(_ID, *this);
	}
};

double Network::evaluateBoard(vector<char> inputBoard) const { 	//***TODO***
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
vector<Network::weightsInLayer> Network::evolve() const { 	// *** TODO *** Not required for Project 2
	vector<weightsInLayer> dummy;
	return dummy;
};
void Network::replaceWithEvolution(const Network & inputNetwork) {
	_layers = std::move(inputNetwork.evolve());
};

bool AI::operator<(const Network & lhs, const Network & rhs) {
	if (lhs.getPerformance() < rhs.getPerformance())
		return true;
	return false;
}

bool AI::operator>(const Network & lhs, const Network & rhs) {
	return rhs < lhs;
}

bool AI::operator<=(const Network & lhs, const Network & rhs) {
	return !(lhs > rhs);
}

bool AI::operator>=(const Network & lhs, const Network & rhs) {
	return !(lhs < rhs);
}

void AI::setupNetwork() {

}

void AI::setupNetwork(const vector<int>& dimensions) {

};

string AI::idToFilename(int ID) {
	string filename = to_string(ID) + ".network";
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