#include "consts.h"
using ai::DEBUG;

#include "network-file-io.h"
using ai::NetworkFileWriter;

#include "network.h"
using ai::Network;

#include "utils.h"
using ai::idToFilename;

#include "consts.h"
using NetworkWeightType = ai::Settings::NetworkWeightType;

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
using std::ios;

void NetworkFileWriter::save(const string &filename,
                             const Network &networkToSave) {
  networkFile.open(filename, ios::out | ios::binary);

  savePerformance(networkToSave._performance);
  saveKingWeight(networkToSave._kingWeight);
  saveKingWeight(networkToSave._pieceCountWeight);
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
  networkFile.write(reinterpret_cast<char *>(&networkPerormance), sizeof(int));
}

void NetworkFileWriter::saveKingWeight(NetworkWeightType kingWeight) {
  networkFile.write(reinterpret_cast<char *>(&kingWeight),
                    sizeof(NetworkWeightType));
}

void NetworkFileWriter::saveDimensions(
    const vector<vector<NetworkWeightType>> &layers) {
  size_t numLayers = layers.size();
  networkFile.write(reinterpret_cast<char *>(&numLayers), sizeof(size_t));

  for (const auto &layer : layers) {
    size_t layerSize = layer.size();

    networkFile.write(reinterpret_cast<char *>(&layerSize), sizeof(size_t));
  }
}

void NetworkFileWriter::saveVector(const vector<NetworkWeightType> &toSave) {
  saveVectorSize(toSave);

  for (auto &w : toSave) {
    networkFile.write((char *)&w, sizeof(NetworkWeightType));
  }
}

void NetworkFileWriter::saveVectorSize(const vector<NetworkWeightType> &layer) {
  size_t size = layer.size();

  networkFile.write(reinterpret_cast<char *>(&size), sizeof(size_t));
}
