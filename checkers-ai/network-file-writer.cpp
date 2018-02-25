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

void NetworkFileWriter::save(const string & filename, const Network & networkToSave) {
    networkFile.open(filename, ios::out | ios::binary);

    savePerformance(networkToSave._performance);
    saveKingWeight(networkToSave._kingWeight);
    saveDimensions(networkToSave._layers);

    for (size_t i = 0; i < networkToSave._weights.size(); ++i) {
        auto weights = networkToSave._weights[i];
        saveVector(weights);

        auto sigmas = networkToSave._sigmas[i];
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

	for (const auto & layer : layers) {
        size_t layerSize = layer.size();

        networkFile.write( (char*)&layerSize, sizeof(size_t));
	}
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

void ai::distribution_out(std::mt19937 & randomNumGenerator)
{
    std::ofstream output;
    output.open("gaussian_out.txt", ios::out | ios::trunc);
    for(auto i = 0; i<1000 ; ++i)
    {
        auto num = getGaussianNumberFromZeroToOne(randomNumGenerator);
        output << num <<','<< '\n';
    }
    output.close();
}



