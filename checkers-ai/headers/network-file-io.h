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

	bool loadNetwork(size_t, Network &);
    class NetworkFileReader {
            std::ifstream networkFile;
        public:
            bool load(const std::string & filename, Network & networkRecievingData);

        private:
            std::vector<Settings::NetworkWeightType> loadVector(size_t currLayerDimension);

            bool inline noMoreLayersInNetworkFile();
            int loadPerformanceFrom();
            Settings::NetworkWeightType loadKingWeightFrom();
            std::vector<size_t> loadDimension();

            std::vector<std::vector<Settings::NetworkWeightType>> getNodesFromDimensions(const std::vector<size_t> & dimensions);
    };

	void saveNetwork(size_t, Network &);
    class NetworkFileWriter {
            std::ofstream networkFile;

        public:
            void save(const std::string & filename, const Network & networkToSave);

        private:
            void saveVectorSize(const std::vector<Settings::NetworkWeightType> & layer);
            void saveVector(const std::vector<Settings::NetworkWeightType> & toSave);
            void savePerformance(int networkPerormance);
            void saveKingWeight(Settings::NetworkWeightType kingWeight);
            void saveDimensions(const std::vector<std::vector<Settings::NetworkWeightType>> & layers);
    };
}

#endif
