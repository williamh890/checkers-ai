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
using std::ios;
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
	// Sizing each layer containing nodes within the _layers vector
    cout << "inputdimensions: ";
	for (unsigned int index = 1; index <= inputdimensions.at(0); ++index) {
		_layers[index-1].resize(inputdimensions.at(index));
        cout << inputdimensions.at(index) << " ";
	} cout << endl;

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
	for (auto i : inputBoard) {
		cout << "i = " << i << endl;
		if (i == ' ')
			continue;
		else if (i == 'r')
			_layers[0][index] = 1;
		else if (i == 'R')
			_layers[0][index] = 1 * _kingWeight;
		else if (i == 'b')
			_layers[0][index] = -1;
		else if (i == 'B')
			_layers[0][index] = -1 * _kingWeight;
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

void saveLayerSize(ofstream & outFile, const vector<double> & layer) {
    unsigned int size = layer.size();
    cout << "writing " << layer.size() << " to file." << endl;
    outFile.write( (char*)&size, sizeof(unsigned int));
}

void saveWeightsForLayerTo(ofstream & outFile, const vector<double> & layer) {

    saveLayerSize(outFile, layer);
    for (auto & w : layer) {
        outFile.write( (char*)&w, sizeof(double));
    }
}

void savePerformance(ofstream & outFile, int networkPerormance) {
    outFile.write( (char*)&networkPerormance, sizeof(int));
}

void saveKingWeight(ofstream & outFile, double kingWeight) {
    outFile.write( (char*)&kingWeight, sizeof(double));
}

void saveDimensions(ofstream & outFile, const vector<vector<double>> & layers) {
    unsigned int numLayers = layers.size();
    outFile.write((char*)&numLayers, sizeof(unsigned int));

    cout << "dimensions: ";
	for (const auto & layer : layers) {
        unsigned int layerSize = layer.size();
        cout << layerSize << " ";
        outFile.write( (char*)&layerSize, sizeof(unsigned int));
	} cout << endl;
}

void ai::saveNetwork(int ID, Network & networkToSave) {
    ofstream outFile;

    auto filename = idToFilename(ID);
    outFile.open(filename, ios::out | ios::binary);

    savePerformance(outFile, networkToSave._performance);
    saveKingWeight(outFile, networkToSave._kingWeight);
    saveDimensions(outFile, networkToSave._layers);

    for (auto & layer : networkToSave._weights) {
        saveWeightsForLayerTo(outFile, layer);
    }

    cout << endl;

    outFile.close(); //Unnecessary
}

vector<double> loadWeightsForLayerFrom(ifstream & inFile, unsigned int currLayerDimension) {
    vector<double> layerWeights;

    for (unsigned int i = 0; i < currLayerDimension; ++i) {
        double weight = 0;
        inFile.read( (char*)&weight, sizeof(double));
        layerWeights.push_back(weight);
    }

    return layerWeights;
}

bool inline noMoreLayersIn(ifstream & inFile) {
    return inFile.eof();
}

int loadPerformanceFrom(ifstream & inFile) {
    int performace = 0;
    inFile.read( (char*)&performace, sizeof(int));
    cout << "Perfomance: " << performace << endl;
    return performace;
};

double loadKingWeightFrom(ifstream & inFile) {
    double kingWeight = 0.;
    inFile.read( (char*)&kingWeight, sizeof(double));
    cout << "King Weight: " << kingWeight << endl;

    return kingWeight;
}

vector<unsigned int> loadDimension(ifstream & inFile) {
    unsigned int numLayers = 0;
    inFile.read((char*)&numLayers, sizeof(unsigned int));

    vector<unsigned int> dimensions;
    cout << "loading dimensions: ";
	for (unsigned int i = 0; i < numLayers; ++i) {
        unsigned int layerSize = 0;
        inFile.read( (char*)&layerSize, sizeof(unsigned int));

        cout << layerSize << " ";
        dimensions.push_back(layerSize);
	} cout << endl;

    return dimensions;
}

vector<vector<double>> getNodesFromDimensions(const vector<unsigned int> & dimensions) {
    vector<vector<double>> nodes;

    for (auto size : dimensions) {
        auto nodeLayer = vector<double>(size, 0);
        nodes.push_back(nodeLayer);
    }

    return nodes;
}


bool ai::loadNetwork(int ID, Network & networkRecievingData) {
    vector<vector<double>> weights;

    cout << "loading network" << endl;
    ifstream inFile (idToFilename(ID), ios::in | ios::binary);
    if (!inFile) {
        cout << "Error opening nn file" << endl;
        return false;
    }

    networkRecievingData._performance = loadPerformanceFrom(inFile);
    networkRecievingData._kingWeight = loadKingWeightFrom(inFile);

    auto dimensions = loadDimension(inFile);
    auto nodes = getNodesFromDimensions(dimensions);
    networkRecievingData._layers = nodes;

    unsigned int currLayerDimension = 0.;
    while(true) {
        inFile.read( (char*)&currLayerDimension, sizeof(unsigned int));

        if(noMoreLayersIn(inFile)) {
            break;
        }
        cout << "Layer Dimension: " << currLayerDimension << endl;

        auto layerWeights = loadWeightsForLayerFrom(inFile, currLayerDimension);
        weights.push_back(layerWeights);
    }
    inFile.close(); // unnecessary, files close automatically when they go out of scope

    networkRecievingData._weights = weights;

    return true;
}
