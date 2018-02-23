#include "headers/consts.h"
using ai::DEBUG;
#include "headers/network-file-io.h"
using  ai::NetworkFileReader;

#include "headers/network.h"
using ai::Network;

#include "headers/utils.h"
using ai::idToFilename;

#include "headers/consts.h"
using NetworkWeightType = ai::Settings::NetworkWeightType;

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

bool NetworkFileReader::load(const string & filename, Network & networkRecievingData) {
    inFile.open(filename, ios::in | ios::binary);
    cout << "reading in network" << endl;
    if (!static_cast<bool>(inFile)) {
        cout << "Error opening nn file" << endl;
        return false;
    }

    networkRecievingData._performance = loadPerformanceFrom();
    networkRecievingData._kingWeight = loadKingWeightFrom();

    auto dimensions = loadDimension();
    auto nodes = getNodesFromDimensions(dimensions);
    networkRecievingData._layers = nodes;

    // READ SIGMAS HERE

    vector<vector<NetworkWeightType>> weights;
    vector<vector<NetworkWeightType>> sigmas;
    unsigned int currLayerDimension = 0.;
    while(true) {
        inFile.read( (char*)&currLayerDimension, sizeof(unsigned int));

        if(noMoreLayersIn()) {
            break;
        }

        auto layerWeights = loadWeightsForLayerFrom(currLayerDimension);

        inFile.read( (char*)&currLayerDimension, sizeof(unsigned int));
        auto sigmaValues = loadWeightsForLayerFrom(currLayerDimension);

        weights.push_back(layerWeights);
        sigmas.push_back(sigmaValues);
    }

    inFile.close();
    networkRecievingData._weights = weights;
    networkRecievingData._sigmas = sigmas;

    return true;
}

int NetworkFileReader::loadPerformanceFrom() {
    int performace = 0;
    inFile.read( (char*)&performace, sizeof(int));

    return performace;
}

NetworkWeightType NetworkFileReader::loadKingWeightFrom() {
    NetworkWeightType kingWeight = 0.;
    inFile.read( (char*)&kingWeight, sizeof(NetworkWeightType));

    return kingWeight;
}

vector<unsigned int> NetworkFileReader::loadDimension() {
    unsigned int numLayers = 0;
    inFile.read((char*)&numLayers, sizeof(unsigned int));

    vector<unsigned int> dimensions;
	for (unsigned int i = 0; i < numLayers; ++i) {
        unsigned int layerSize = 0;
        inFile.read( (char*)&layerSize, sizeof(unsigned int));

        dimensions.push_back(layerSize);
	}

    return dimensions;
}

vector<vector<NetworkWeightType>> NetworkFileReader::getNodesFromDimensions(const vector<unsigned int> & dimensions) {
    vector<vector<NetworkWeightType>> nodes;

    for (auto size : dimensions) {
        auto nodeLayer = vector<NetworkWeightType>(size, 0);
        nodes.push_back(nodeLayer);
    }

    return nodes;
}

vector<NetworkWeightType> NetworkFileReader::loadWeightsForLayerFrom(unsigned int currLayerDimension) {
    vector<NetworkWeightType> layerWeights;

    for (unsigned int i = 0; i < currLayerDimension; ++i) {
        NetworkWeightType weight = 0;
        inFile.read( (char*)&weight, sizeof(NetworkWeightType));
        layerWeights.push_back(weight);
    }

    return layerWeights;
}


bool inline NetworkFileReader::noMoreLayersIn() {
    return inFile.eof();
}







