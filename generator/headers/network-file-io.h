#ifndef NETWORK_FILE_IO_H_INCLUDED
#define NETWORK_FILE_IO_H_INCLUDED

#include "network.h"
// ai::Network

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
            std::vector<double> loadWeightsForLayerFrom(unsigned int currLayerDimension);

            bool inline noMoreLayersIn();
            int loadPerformanceFrom();
            double loadKingWeightFrom();
            std::vector<unsigned int> loadDimension();

            std::vector<std::vector<double>> getNodesFromDimensions(const std::vector<unsigned int> & dimensions);
    };

	void saveNetwork(unsigned int, Network &);
    class NetworkFileWriter {
            std::ofstream outFile;

        public:
            void save(const std::string & filename, const Network & networkToSave);

        private:
            void saveLayerSize(const std::vector<double> & layer);
            void saveWeightsForLayerTo(const std::vector<double> & layer);
            void savePerformance(int networkPerormance);
            void saveKingWeight(double kingWeight);
            void saveDimensions(const std::vector<std::vector<double>> & layers);
    };
}

#endif
