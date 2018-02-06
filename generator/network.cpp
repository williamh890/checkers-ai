#include "headers\network.h"
using ai::Network;

#include <vector>
using std::vector;
using std::swap;

Network::Network(int inputID): _ID(inputID) {
	loadWeightsFromFile();
};

Network::Network(const vector<int> & inputdimensions) {
	setupNetwork(inputdimensions);
}

Network::~Network() {
	if (_gameCompleted) {
		saveWeightsToFile();
	}
};

void Network::setupNetwork() {

}

void Network::setupNetwork(const vector<int>& dimensions) {

};

void Network::loadWeightsFromFile() { //*** TODO ***
	// I intend to implement this using boost::serialization
};

void Network::saveWeightsToFile() { 	//***TODO***
	// I intend to implement this using boost::serialization
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

vector<vector<double>> & Network::evolve() const { 	// *** TODO ***
	vector<weightsInLayer> dummy;
	return dummy;
};

void Network::replaceWithEvolution(vector<weightsInLayer> & inputNetwork) {
	swap(_layers, inputNetwork);
};

bool operator<(const Network & lhs, const Network & rhs) {
	if (lhs.getPerformance() < rhs.getPerformance())
		return true;
	return false;
}

bool operator>(const Network & lhs, const Network & rhs) {
	return rhs < lhs;
}

bool operator<=(const Network & lhs, const Network & rhs) {
	return !(lhs > rhs);
}

bool operator>=(const Network & lhs, const Network & rhs) {
	return !(lhs < rhs);
}
