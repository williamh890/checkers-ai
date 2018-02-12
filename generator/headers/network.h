#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "seeder.h"
// ai::Seeder
#include "network-file-io.h"
// ai::NetworkFileReader
// ai::NetworkFileWriter
#include "consts.h"
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
		using layersContainingNodes = std::vector<double>;
		using networkWeights = std::vector<double>;

		Network(unsigned int);
		Network(const std::vector<unsigned int>&, unsigned int, std::shared_ptr<Seeder> & seeder); // This should probably be made private
		~Network();

		double evaluateBoard (const std::vector<char> &);
		void adjustPerformance(int result);
		int getPerformance() const;
		void resetPerformance();

		std::vector<networkWeights> evolve() const; // *** TODO *** Not required for Project 2
		void replaceWithEvolution(const Network &);

		void outputCreationDebug();

	private:
		unsigned int _ID;
		std::vector<layersContainingNodes> _layers;
		std::vector<networkWeights> _weights;
		double _kingWeight;
		int _performance;
		bool _gameCompleted = false;

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
