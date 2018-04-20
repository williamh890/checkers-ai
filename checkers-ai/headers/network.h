#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

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
            static void save(size_t id, Network & toSave);
            static bool load(size_t id, Network & toLoad);
            static bool load(std::string & filename, Network & toLoad);

            using LayersContainingNodes = std::vector<Settings::NetworkWeightType>;
            using NetworkWeights = std::vector<Settings::NetworkWeightType>;

            Network () = default;
            Network (unsigned int id);
            Network (unsigned int id,
                     const std::vector<unsigned int> & LayerDimension,
                     std::shared_ptr<Seeder> & seeder);
            Network(const Network & other) = default;
            Network(Network && other) = default;
            Network & operator= (const Network &rhs) = default;
            Network & operator= (Network && rhs) = default;

            ~Network();

            Settings::NetworkWeightType evaluateBoard(const std::vector<char> &, bool leave_Out_Activator = false, int red_factor = 1);

            void adjustPerformance(int result);
            int getPerformance() const;
            void resetPerformance();

            void evolveUsingNetwork(ai::Network &);
            void evolve();
            unsigned int getID() const;

            void outputCreationDebug() const;
            void changeKingWeight(Settings::NetworkWeightType);
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

            void setupLayers(const std::vector<unsigned int> & LayerDimensions);
            void setupRandomWeights(const std::vector<unsigned int> & LayerDimensions);
            void setupKingWeight();
            void setupSigmas();
            void setupwhichLayerofNetworkToUse();
            int getNumLayers();
            void getLayerSizes(std::vector<int> &);

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

            void calculateNode(unsigned int, unsigned int);
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

            friend bool nothingSimilar(const Network &, const Network &);
            friend bool operator==(const Network &, const Network &);
            friend void weightChangeOut(Network parent, Network child);
            friend bool validateNetworks();
    }; // end class AI_Network

    // Global operators to allow sorting of networks based on their performance
    bool operator< (const Network &lhs, const Network &rhs);
    bool operator> (const Network &lhs, const Network &rhs);
    bool operator<= (const Network &lhs, const Network &rhs);
    bool operator>= (const Network &lhs, const Network &rhs);
    bool operator== (const Network &lhs, const Network &rhs);

    bool nothingSimilar(const Network &, const Network &);
    void weightChangeOut(Network parent, Network child);
    void setupNetworks (const std::vector<unsigned int> & dimensions, int numberOfNetworks = NETWORKPOPSIZE);
    Settings::NetworkWeightType getGaussianNumber(std::mt19937 &);
    bool validateNetworks();
}
#endif // NETWORK_H_INCLUDED
