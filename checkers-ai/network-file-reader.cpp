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

bool NetworkFileReader::load(const string & filename, Network & networkRecievingData) {
    networkFile.open(filename, ios::in | ios::binary);

    if (!static_cast<bool>(networkFile)) {
        cout << "Error opening nn file" << endl;
        return false;
    }

    networkRecievingData._performance = loadPerformanceFrom();
    networkRecievingData._kingWeight = loadKingWeightFrom();

    auto dimensions = loadDimension();
    auto nodes = getNodesFromDimensions(dimensions);
    cout<<" LOADING NETWORK, WE HAVE "<<nodes.size()<<" LAYERS"<<endl;
    networkRecievingData._layers = nodes;

    vector<vector<NetworkWeightType>> weights;
    vector<vector<NetworkWeightType>> sigmas;

    size_t vectorSize = 0.;
    while(true) {
        networkFile.read( (char*)&vectorSize, sizeof(size_t));

        if(noMoreLayersInNetworkFile()) {
            break;
        }

        auto layerWeights = loadVector(vectorSize);

        networkFile.read( (char*)&vectorSize, sizeof(size_t));
        auto sigmaValues = loadVector(vectorSize);

        weights.push_back(layerWeights);
        sigmas.push_back(sigmaValues);
    }

    networkFile.close();

    networkRecievingData._weights = weights;
    networkRecievingData._sigmas = sigmas;
    return true;
}

int NetworkFileReader::loadPerformanceFrom() {
    int performace = 0;
    networkFile.read( (char*)&performace, sizeof(int));

    return performace;
}

NetworkWeightType NetworkFileReader::loadKingWeightFrom() {
    NetworkWeightType kingWeight = 0.;
    networkFile.read( (char*)&kingWeight, sizeof(NetworkWeightType));

    return kingWeight;
}

vector<size_t>
NetworkFileReader::loadDimension() {
    size_t numLayers = 0;
    networkFile.read((char*)&numLayers, sizeof(size_t));

    vector<size_t> dimensions;
	for (size_t i = 0; i < numLayers; ++i) {
        size_t layerSize = 0;
        networkFile.read( (char*)&layerSize, sizeof(size_t));

        dimensions.push_back(layerSize);
	}

    return dimensions;
}

vector<vector<NetworkWeightType>>
NetworkFileReader::getNodesFromDimensions(const vector<size_t> & dimensions) {
    vector<vector<NetworkWeightType>> nodes;

    for (auto size : dimensions) {
        auto nodeLayer = vector<NetworkWeightType>(size, 0);
        nodes.push_back(nodeLayer);
    }

    return nodes;
}

vector<NetworkWeightType>
NetworkFileReader::loadVector(size_t currLayerDimension) {
    vector<NetworkWeightType> layerWeights;

    for (size_t i = 0; i < currLayerDimension; ++i) {
        NetworkWeightType weight = 0;
        networkFile.read( (char*)&weight, sizeof(NetworkWeightType));
        layerWeights.push_back(weight);
    }

    return layerWeights;
}

bool inline NetworkFileReader::noMoreLayersInNetworkFile() {
    return networkFile.eof();
}
