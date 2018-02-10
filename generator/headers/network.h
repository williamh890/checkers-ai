#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "seeder.h"
// ai::Seeder

#include <vector>
// std::vector
#include <fstream>
// std::???
#include <iostream>
// std::???
#include <string>
// std::string

namespace ai {
	class Network {
		bool DEBUG = true;
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

		//friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive &ar, const unsigned int version) {
			ar & _ID;
			ar & _layers;
			ar & _weights;
			ar & _kingWeight;
			ar & _performance;
		}
        friend void saveNetwork(int, Network &);
        friend bool loadNetwork(int, Network &);

		void calculateNode(unsigned int, unsigned int);
	}; // end class AI_Network

	// Global operators to allow sorting of networks based on their performance
	bool operator< (const Network &lhs, const Network &rhs);
	bool operator> (const Network &lhs, const Network &rhs);
	bool operator<= (const Network &lhs, const Network &rhs);
	bool operator>= (const Network &lhs, const Network &rhs);

	void setupNetworks (const std::vector<unsigned int> & dimesions, int numberOfNetworks = 100);

	// Functions to handle saving and loading of networks utilizing boost serialization
	std::string idToFilename(int);
	void saveNetwork(int, Network &);
	bool loadNetwork(int, Network &);
}
#endif // NETWORK_H_INCLUDED
