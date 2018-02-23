#ifndef NETWORK_FILE_IO_H_INCLUDED
#define NETWORK_FILE_IO_H_INCLUDED

#include "network.h"
// ai::NetworkWeightType
// ai::Network

#include "consts.h"
// Settings::NetworkWeightType

#include <fstream>
// std::ofstream;
// std::ifstream;
#include <vector>
// std::vector

namespace ai {
    class Network;

	bool loadNetwork(unsigned int, Network &);
    class NetworkFileReader {
            std::ifstream inFile;
        public:
            bool load(const std::string & filename, Network & networkRecievingData);

        private:
            std::vector<Settings::NetworkWeightType> loadWeightsForLayerFrom(unsigned int currLayerDimension);
            std::vector<Settings::NetworkWeightType> loadSigmasForLayerFrom(unsigned int currLayerDimension);

            bool inline noMoreLayersIn();
            int loadPerformanceFrom();
            Settings::NetworkWeightType loadKingWeightFrom();
            std::vector<unsigned int> loadDimension();

            std::vector<std::vector<Settings::NetworkWeightType>> getNodesFromDimensions(const std::vector<unsigned int> & dimensions);
    };

	void saveNetwork(unsigned int, Network &);
    class NetworkFileWriter {
            std::ofstream outFile;

        public:
            void save(const std::string & filename, const Network & networkToSave);

        private:
            void saveLayerSize(const std::vector<Settings::NetworkWeightType> & layer);
            void saveWeightsForLayerTo(const std::vector<Settings::NetworkWeightType> & layer);
            void saveSigmasForLayerTo(const std::vector<Settings::NetworkWeightType> & layer);
            void savePerformance(int networkPerormance);
            void saveKingWeight(Settings::NetworkWeightType kingWeight);
            void saveDimensions(const std::vector<std::vector<Settings::NetworkWeightType>> & layers);
    };
}

#endif
