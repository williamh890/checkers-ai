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
		using layersContainingNodes = std::vector<Settings::NetworkWeightType>;
		using networkWeights = std::vector<Settings::NetworkWeightType>;

		Network(unsigned int id);
		Network(const std::vector<unsigned int>& layerDimension,
                unsigned int networkId,
                std::shared_ptr<Seeder> & seeder); // This should probably be made private

		~Network();

    private:
        void setupLayers(const std::vector<unsigned int> & layerDimensions);
        void setupRandomWeights(const std::vector<unsigned int> & layerDimensions);
        void setupKingWeight();

        template <typename RandomNumberType>
        std::vector<RandomNumberType>
        getRandomNumbersOfLength(
                const unsigned int length,
                std::uniform_real_distribution<RandomNumberType> & distribution);

    public:
		double evaluateBoard (const std::vector<char> &, bool leave_Out_Activator = false);
		void adjustPerformance(int result);
		int getPerformance() const;
		void resetPerformance();

		std::vector<networkWeights> evolve() const; // *** TODO *** Not required for Project 2
		void replaceWithEvolution(const Network &);

		void outputCreationDebug();
		void changeKingWeight(double);

	private:
		unsigned int _ID;
		std::vector<layersContainingNodes> _layers;
		std::vector<networkWeights> _weights;
		double _kingWeight;
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
