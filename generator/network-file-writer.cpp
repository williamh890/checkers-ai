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


void ai::saveNetwork(unsigned int id, Network & network) {
    auto filename = idToFilename(id);
    NetworkFileWriter writer;

    writer.save(filename, network);
}

void NetworkFileWriter::saveLayerSize(const vector<NetworkWeightType> & layer) {
    unsigned int size = layer.size();
    cout << "writing " << layer.size() << " to file." << endl;
    outFile.write( (char*)&size, sizeof(unsigned int));
}

void NetworkFileWriter::saveWeightsForLayerTo(const vector<NetworkWeightType> & layer) {

    saveLayerSize(layer);
    for (auto & w : layer) {
        outFile.write( (char*)&w, sizeof(NetworkWeightType));
    }
}

void NetworkFileWriter::savePerformance(int networkPerormance) {
    outFile.write( (char*)&networkPerormance, sizeof(int));
}

void NetworkFileWriter::saveKingWeight(double kingWeight) {
    outFile.write( (char*)&kingWeight, sizeof(NetworkWeightType));
}

void NetworkFileWriter::saveDimensions(const vector<vector<NetworkWeightType>> & layers) {
    unsigned int numLayers = layers.size();
    outFile.write((char*)&numLayers, sizeof(unsigned int));

    cout << "dimensions: ";
	for (const auto & layer : layers) {
        unsigned int layerSize = layer.size();
        cout << layerSize << " ";
        outFile.write( (char*)&layerSize, sizeof(unsigned int));
	} cout << endl;
}

void NetworkFileWriter::save(const string & filename, const Network & networkToSave) {

    outFile.open(filename, ios::out | ios::binary);

    savePerformance(networkToSave._performance);
    saveKingWeight(networkToSave._kingWeight);
    saveDimensions(networkToSave._layers);

    for (auto & layer : networkToSave._weights) {
        saveWeightsForLayerTo(layer);
    }

    cout << endl;

    outFile.close(); //Unnecessary
}


