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

namespace ai {
    class NetworkFileReader;
    class NetworkFileWriter;

    class Network {
        public:
            static void save(size_t id, Network & toSave);
            static bool load(size_t id, Network & toLoad);

            using LayersContainingNodes = std::vector<Settings::NetworkWeightType>;
            using NetworkWeights = std::vector<Settings::NetworkWeightType>;

            Network (unsigned int id);
            Network (unsigned int id,
                     const std::vector<unsigned int> & LayerDimension,
                     std::shared_ptr<Seeder> & seeder);
            ~Network();

            Settings::NetworkWeightType evaluateBoard(const std::vector<char> &, bool leave_Out_Activator = false);

            void adjustPerformance(int result);
            int getPerformance() const;
            void resetPerformance();

            void evolveUsingNetwork(const ai::Network &);

            void outputCreationDebug();
            void changeKingWeight(Settings::NetworkWeightType);
        private:
            unsigned int _ID;
            std::vector<LayersContainingNodes> _layers;
            std::vector<NetworkWeights> _weights;
            std::vector<NetworkWeights> _sigmas;
            Settings::NetworkWeightType _kingWeight;
            int _performance;
            bool _gameCompleted = false;
            std::mt19937 randomNumGenerator;

            void setupLayers(const std::vector<unsigned int> & LayerDimensions);
            void setupRandomWeights(const std::vector<unsigned int> & LayerDimensions);
            void setupKingWeight();
            void setupSigmas();

            void evolve();
            Settings::NetworkWeightType getTau();
            void evolveKingWeight();

            void evolveSigmas();

            Settings::NetworkWeightType
            inline evolveSigmaAt(size_t i, size_t ii, size_t tau);

            void evolveWeights();

            Settings::NetworkWeightType
            inline evolveWeightAt(size_t i, size_t ii);

            template <typename RandomNumberType>
                std::vector<RandomNumberType> getRandomNumbersOfLength(const unsigned int length,
                                        std::uniform_real_distribution<RandomNumberType> & distribution);

            void calculateNode(unsigned int, unsigned int);
            inline Settings::NetworkWeightType activationFunction(Settings::NetworkWeightType x);

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
    Settings::NetworkWeightType getGaussianNumberFromZeroToOne(std::mt19937 &);
}
#endif // NETWORK_H_INCLUDED
