#include "headers/consts.h"
using ai::DEBUG;

#include "headers/network-file-io.h"
using ai::NetworkFileWriter;

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


void ai::saveNetwork(unsigned int id, Network & network) {
    auto filename = idToFilename(id);
    NetworkFileWriter writer;

    writer.save(filename, network);
}

void NetworkFileWriter::saveLayerSize(const vector<double> & layer) {
    unsigned int size = layer.size();
    if (DEBUG)
        cout << "writing " << layer.size() << " to file." << endl;
    outFile.write( (char*)&size, sizeof(unsigned int));
}

void NetworkFileWriter::saveWeightsForLayerTo(const vector<double> & layer) {

    saveLayerSize(layer);
    for (auto & w : layer) {
        outFile.write( (char*)&w, sizeof(double));
    }
}

void NetworkFileWriter::savePerformance(int networkPerormance) {
    outFile.write( (char*)&networkPerormance, sizeof(int));
}

void NetworkFileWriter::saveKingWeight(double kingWeight) {
    outFile.write( (char*)&kingWeight, sizeof(double));
}

void NetworkFileWriter::saveDimensions(const vector<vector<double>> & layers) {
    unsigned int numLayers = layers.size();
    outFile.write((char*)&numLayers, sizeof(unsigned int));

    if (DEBUG)
        cout << "dimensions: ";
	for (const auto & layer : layers) {
        unsigned int layerSize = layer.size();
        if (DEBUG)
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

    if (DEBUG)
        cout << endl;

    outFile.close(); //Unnecessary
}


