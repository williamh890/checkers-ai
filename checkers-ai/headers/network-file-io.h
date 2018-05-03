#ifndef NETWORK_FILE_IO_H
#define NETWORK_FILE_IO_H

#include "consts.h"
// ai::Settings::NetworkWeightType

#include "network.h"
// ai::NetworkWeightType
// ai::Network

#include <fstream>
// std::ofstream;
// std::ifstream;
#include <vector>
// std::vector

namespace ai {
class Network;

class NetworkFileReader {
  std::ifstream networkFile;

 public:
  bool load(const std::string& filename, Network& networkRecievingData);

 private:
  std::vector<ai::Settings::NetworkWeightType> loadVector(
      size_t currLayerDimension);

  bool inline noMoreLayersInNetworkFile();
  int loadPerformanceFrom();
  ai::Settings::NetworkWeightType loadKingWeightFrom();
  std::vector<size_t> loadDimension();

  std::vector<std::vector<ai::Settings::NetworkWeightType>>
  getNodesFromDimensions(const std::vector<size_t>& dimensions);
};

class NetworkFileWriter {
  std::ofstream networkFile;

 public:
  void save(const std::string& filename, const Network& networkToSave);

 private:
  void saveVectorSize(
      const std::vector<ai::Settings::NetworkWeightType>& layer);
  void saveVector(const std::vector<ai::Settings::NetworkWeightType>& toSave);
  void savePerformance(int networkPerormance);
  void saveKingWeight(ai::Settings::NetworkWeightType kingWeight);
  void saveDimensions(
      const std::vector<std::vector<ai::Settings::NetworkWeightType>>& layers);
};
void distribution_out(int out_num);

}  // namespace ai

#endif  // NETWORK_FILE_IO_H
