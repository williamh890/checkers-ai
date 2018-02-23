#include "headers/consts.h"
using ai::DEBUG;

#include "headers/network-file-io.h"
using ai::NetworkFileWriter;

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


void ai::saveNetwork(size_t id, Network & network) {
    auto filename = idToFilename(id);
    NetworkFileWriter writer;

    writer.save(filename, network);
}

void NetworkFileWriter::save(const string & filename, const Network & networkToSave) {
    networkFile.open(filename, ios::out | ios::binary);

    savePerformance(networkToSave._performance);
    saveKingWeight(networkToSave._kingWeight);
    saveDimensions(networkToSave._layers);

    for (size_t i = 0; i < networkToSave._weights.size(); ++i) {
        auto sigmas = networkToSave._sigmas[i];
        auto weights = networkToSave._weights[i];

        saveVector(weights);
        saveVector(sigmas);
    }

    networkFile.close();
}

void NetworkFileWriter::savePerformance(int networkPerormance) {
    networkFile.write( (char*)&networkPerormance, sizeof(int));
}

void NetworkFileWriter::saveKingWeight(NetworkWeightType kingWeight) {
    networkFile.write( (char*)&kingWeight, sizeof(NetworkWeightType));
}

void NetworkFileWriter::saveDimensions(const vector<vector<NetworkWeightType>> & layers) {
    size_t numLayers = layers.size();
    networkFile.write((char*)&numLayers, sizeof(size_t));

    if (DEBUG)
        cout << "dimensions: ";
	for (const auto & layer : layers) {
        size_t layerSize = layer.size();
        if (DEBUG)
            cout << layerSize << " ";
        networkFile.write( (char*)&layerSize, sizeof(size_t));
	}

    if (DEBUG)
        cout << endl;
}

void NetworkFileWriter::saveVector(const vector<NetworkWeightType> & toSave) {
    saveVectorSize(toSave);

    for (auto & w : toSave) {
        networkFile.write( (char*)&w, sizeof(NetworkWeightType));
    }
}

void NetworkFileWriter::saveVectorSize(const vector<NetworkWeightType> & layer) {
    size_t size = layer.size();

    networkFile.write( (char*)&size, sizeof(size_t));
}



