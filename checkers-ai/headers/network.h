#ifndef NETWORK_H
#define NETWORK_H

#include "consts.h"
// ai::Settings::NetworkWeightType

#include "seeder.h"
// ai::Seeder

#include "network-file-io.h"
// ai::NetworkFileReader
// ai::NetworkFileWriter

#include <random>
// std::uniform_real_distribution
#include <vector>
// std::vector
#include <string>
// std::string

namespace ai {
    class NetworkFileReader;
    class NetworkFileWriter;

    class Network {
        public:
            static void save(size_t networkId, Network & networkToSave);
            static bool load(size_t networkId, Network & networkToLoad);
            static bool load(std::string & filename, Network & networkToLoad);

            using LayersContainingNodes = std::vector<Settings::NetworkWeightType>;
            using NetworkWeights = std::vector<Settings::NetworkWeightType>;

            Network () = default;
            explicit Network (unsigned int networkId);
            Network (unsigned int networkId,
                     const std::vector<unsigned int> & layerDimensions,
                     std::shared_ptr<Seeder> & seeder);
            Network(const Network & other) = default;
            Network(Network && other) noexcept  = default;
            Network & operator= (const Network &rhs) = default;
            Network & operator= (Network && rhs) noexcept = default;

            ~Network();

            Settings::NetworkWeightType evaluateBoard(const std::vector<char> & /*inputBoard*/, 
                                                      bool testing = false, int red_factor = 1);

            void adjustPerformance(int adjustment);
            int getPerformance() const;
            void resetPerformance();

            void evolveUsingNetwork(ai::Network & /*rhs*/);
            void evolve();
            unsigned int getID() const;

            void outputCreationDebug() const;
            void changeKingWeight(Settings::NetworkWeightType /*newWeight*/);
        private:
            unsigned int _ID;
            std::vector<LayersContainingNodes> _layers;
            std::vector<NetworkWeights> _weights;
            std::vector<NetworkWeights> _sigmas;
            Settings::NetworkWeightType _kingWeight;
            Settings::NetworkWeightType _pieceCountWeight;
            std::vector<std::vector<unsigned int>> _whichLayerofNetworkToUse;
            int _performance;
            bool _gameCompleted = false;
            std::mt19937 randomNumGenerator;

            void setupLayers(const std::vector<unsigned int> & layerDimensions);
            void setupRandomWeights(const std::vector<unsigned int> & layerDimensions);
            void setupKingWeight();
            void setupSigmas();
            void setupwhichLayerofNetworkToUse();
            int getNumLayers();
            void getLayerSizes(std::vector<int> & /*layerSizes*/);

            Settings::NetworkWeightType getTau();
            void evolveKingWeight();
            void evolveSigmas();
            void inline evolveSigmaAt(size_t i, size_t ii, Settings::NetworkWeightType tau);
            void evolveWeights();
            Settings::NetworkWeightType
            inline evolveWeightAt(size_t i, size_t ii);

            template <typename RandomNumberType>
                std::vector<RandomNumberType> getRandomNumbersOfLength(const unsigned int length,
                                        std::uniform_real_distribution<RandomNumberType> & distribution);

            void calculateNode(unsigned int /*x*/, unsigned int /*y*/);
            inline Settings::NetworkWeightType activationFunction(Settings::NetworkWeightType x);

            void networkSwap(Network& rhs) {
                std::swap(_weights, rhs._weights);
                std::swap(_sigmas, rhs._sigmas);
                std::swap(_kingWeight, rhs._kingWeight);
                std::swap(_performance, rhs._performance);
            }

        public:
            friend class NetworkFileReader;
            friend class NetworkFileWriter;

            friend bool nothingSimilar(const Network & /*lhs*/, const Network & /*rhs*/);
            friend bool operator==(const Network & /*lhs*/, const Network & /*rhs*/);
            friend void weightChangeOut(Network parent, Network child);
            friend bool validateNetworks();
    }; // end class AI_Network

    // Global operators to allow sorting of networks based on their performance
    bool operator< (const Network &lhs, const Network &rhs);
    bool operator> (const Network &lhs, const Network &rhs);
    bool operator<= (const Network &lhs, const Network &rhs);
    bool operator>= (const Network &lhs, const Network &rhs);
    bool operator== (const Network &lhs, const Network &rhs);

    bool nothingSimilar(const Network & /*lhs*/, const Network & /*rhs*/);
    void weightChangeOut(Network parent, Network child);
    void setupNetworks (const std::vector<unsigned int> & dimensions, int numberOfNetworks = NETWORKPOPSIZE);
    Settings::NetworkWeightType getGaussianNumber(std::mt19937 & /*randomNumGenerator*/);
    bool validateNetworks();
} // namespace ai
#endif // NETWORK_H
