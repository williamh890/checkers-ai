#include "headers\network.h"
#include <vector>
using std::vector;
using std::swap;

AI_Network::AI_Network(int inputID): _ID(inputID) {
	loadWeightsFromFile();
};

AI_Network::AI_Network(const vector<int> & inputdimensions) {
	setupNetwork(inputdimensions);
}

AI_Network::~AI_Network() {
	if (_gameCompleted) {
		saveWeightsToFile();
	}
};

void AI_Network::setupNetwork() {

}

void AI_Network::setupNetwork(const vector<int>& dimensions) {

};

void AI_Network::loadWeightsFromFile() { //*** TODO ***
	// I intend to implement this using boost::serialization
};

void AI_Network::saveWeightsToFile() { 	//***TODO***
	// I intend to implement this using boost::serialization
};

double AI_Network::evaluateBoard(vector<char> inputBoard) const { 	//***TODO***
	double dummy = 0;
	return dummy;
};
void AI_Network::adjustPerformance(int resultFromGame) {
	_performance += resultFromGame;
	_gameCompleted = true;
};

int AI_Network::getPerformance() const {
	return _performance;
}
void AI_Network::resetPerformance() {
	_performance = 0;
};
vector<vector<double>> & AI_Network::evolve() const { 	// *** TODO ***
	vector<weightsInLayer> dummy;
	return dummy;
};
void AI_Network::replaceWithEvolution(vector<weightsInLayer> & inputNetwork) {
	swap(_layers, inputNetwork);
};

bool operator<(const AI_Network & lhs, const AI_Network & rhs) {
	if (lhs.getPerformance() < rhs.getPerformance())
		return true;
	return false;
}

bool operator>(const AI_Network & lhs, const AI_Network & rhs) {
	return rhs < lhs;
}

bool operator<=(const AI_Network & lhs, const AI_Network & rhs) {
	return !(lhs > rhs);
}

bool operator>=(const AI_Network & lhs, const AI_Network & rhs) {
	return !(lhs < rhs);
}
