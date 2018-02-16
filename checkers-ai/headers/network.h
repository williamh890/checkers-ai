#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "seeder.h"
// ai::Seeder
#include "network-file-io.h"
// ai::NetworkFileReader
// ai::NetworkFileWriter
#include "consts.h"
// ai::Settings::NetworkWeightType

#include <random>
// std::uniform_real_distribution
#include <vector>
// std::vector
#include <fstream>
// std::???
#include <iostream>
// std::???
#include <string>
// std::string

namespace ai {
    class NetworkFileReader;
    class NetworkFileWriter;

    class Network {
        public:
            using LayersContainingNodes = std::vector<Settings::NetworkWeightType>;
            using NetworkWeights = std::vector<Settings::NetworkWeightType>;

            Network(unsigned int id);
            Network(unsigned int id,
                    const std::vector<unsigned int> & LayerDimension,
                    std::shared_ptr<Seeder> & seeder); // This should probably be made private

            ~Network();

        private:
            void setupLayers(const std::vector<unsigned int> & LayerDimensions);
            void setupRandomWeights(const std::vector<unsigned int> & LayerDimensions);
            void setupKingWeight();

            template <typename RandomNumberType>
                std::vector<RandomNumberType>
                getRandomNumbersOfLength(
                        const unsigned int length,
                        std::uniform_real_distribution<RandomNumberType> & distribution);

        public:
            Settings::NetworkWeightType evaluateBoard(const std::vector<char> &, bool leave_Out_Activator = false);
            inline Settings::NetworkWeightType activationFunction(Settings::NetworkWeightType x);

            void adjustPerformance(int result);
            int getPerformance() const;
            void resetPerformance();

            std::vector<NetworkWeights> evolve() const; // *** TODO *** Not required for Project 2
            void replaceWithEvolution(const Network &);

            void outputCreationDebug();
            void changeKingWeight(Settings::NetworkWeightType);
        private:
            unsigned int _ID;
            std::vector<LayersContainingNodes> _layers;
            std::vector<NetworkWeights> _weights;
            Settings::NetworkWeightType _kingWeight;
            int _performance;
            bool _gameCompleted = false;
            std::mt19937 randomNumGenerator;

            void calculateNode(unsigned int, unsigned int);

        public:
            friend class NetworkFileReader;
            friend class NetworkFileWriter;
            friend bool operator==(const Network &, const Network &);
    }; // end class AI_Network

    // Global operators to allow sorting of networks based on their performance
    bool operator< (const Network &lhs, const Network &rhs);
    bool operator> (const Network &lhs, const Network &rhs);
    bool operator<= (const Network &lhs, const Network &rhs);
    bool operator>= (const Network &lhs, const Network &rhs);
    bool operator== (const Network &lhs, const Network &rhs);

    void setupNetworks (const std::vector<unsigned int> & dimesions, int numberOfNetworks = 100);
}
#endif // NETWORK_H_INCLUDED
