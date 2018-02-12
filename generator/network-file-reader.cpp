#include "headers/consts.h"
using ai::DEBUG;
#include "headers/network-file-io.h"
using  ai::NetworkFileReader;

#include "headers/network.h"
using ai::Network;

#include "headers/utils.h"
using ai::idToFilename;

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ios;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;

bool ai::loadNetwork(unsigned int id, Network & network) {
    auto filename = idToFilename(id);
    NetworkFileReader reader;

    return reader.load(filename, network);
}

vector<double> NetworkFileReader::loadWeightsForLayerFrom(unsigned int currLayerDimension) {
    vector<double> layerWeights;

    for (unsigned int i = 0; i < currLayerDimension; ++i) {
        double weight = 0;
        inFile.read( (char*)&weight, sizeof(double));
        layerWeights.push_back(weight);
    }

    return layerWeights;
}

bool inline NetworkFileReader::noMoreLayersIn() {
    return inFile.eof();
}

int NetworkFileReader::loadPerformanceFrom() {
    int performace = 0;
    inFile.read( (char*)&performace, sizeof(int));
    if (DEBUG)
        cout << "Perfomance: " << performace << endl;
    return performace;
};

double NetworkFileReader::loadKingWeightFrom() {
    double kingWeight = 0.;
    inFile.read( (char*)&kingWeight, sizeof(double));
    if (DEBUG)
        cout << "King Weight: " << kingWeight << endl;

    return kingWeight;
}

vector<unsigned int> NetworkFileReader::loadDimension() {
    unsigned int numLayers = 0;
    inFile.read((char*)&numLayers, sizeof(unsigned int));

    vector<unsigned int> dimensions;
    if (DEBUG)
        cout << "loading dimensions: ";
	for (unsigned int i = 0; i < numLayers; ++i) {
        unsigned int layerSize = 0;
        inFile.read( (char*)&layerSize, sizeof(unsigned int));

        if (DEBUG)
            cout << layerSize << " ";
        dimensions.push_back(layerSize);
	}
    if (DEBUG)
        cout << endl;

    return dimensions;
}

vector<vector<double>> NetworkFileReader::getNodesFromDimensions(const vector<unsigned int> & dimensions) {
    vector<vector<double>> nodes;

    for (auto size : dimensions) {
        auto nodeLayer = vector<double>(size, 0);
        nodes.push_back(nodeLayer);
    }

    return nodes;
}


bool NetworkFileReader::load(const string & filename, Network & networkRecievingData) {
    vector<vector<double>> weights;
    if (DEBUG)
        cout << "loading network" << endl;
    inFile.open(filename, ios::in | ios::binary);
    if (!inFile) {
        cout << "Error opening nn file" << endl;
        return false;
    }

    networkRecievingData._performance = loadPerformanceFrom();
    networkRecievingData._kingWeight = loadKingWeightFrom();

    auto dimensions = loadDimension();
    auto nodes = getNodesFromDimensions(dimensions);
    networkRecievingData._layers = nodes;

    unsigned int currLayerDimension = 0.;
    while(true) {
        inFile.read( (char*)&currLayerDimension, sizeof(unsigned int));

        if(noMoreLayersIn()) {
            break;
        }
        if (DEBUG)
            cout << "Layer Dimension: " << currLayerDimension << endl;

        auto layerWeights = loadWeightsForLayerFrom(currLayerDimension);
        weights.push_back(layerWeights);
    }

    inFile.close();
    networkRecievingData._weights = weights;

    return true;
}

