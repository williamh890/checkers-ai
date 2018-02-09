#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <vector>

#include "boost\archive\basic_text_iarchive.hpp"
#include "boost\archive\basic_text_oarchive.hpp"
#include "boost\serialization\vector.hpp"
#include <fstream>
#include <iostream>
#include <string>
namespace AI {
	class Network {
	public:
		using weightsInLayer = std::vector<double>;

		Network(int);
		Network(const std::vector<int>&);
		~Network();

		double evaluateBoard(std::vector<char>) const; // *** TODO ***
		void adjustPerformance(int result);
		int getPerformance() const;
		void resetPerformance();

		std::vector<weightsInLayer> evolve() const; // *** TODO *** Not required for Project 2
		void replaceWithEvolution(const Network &);

	private:
		int _ID;
		std::vector<weightsInLayer> _layers;
		int _performance;
		bool _gameCompleted = true;

		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive &ar, const unsigned int version) {
			ar & _ID;
			ar & _layers;
			ar & _performance;
		}

	}; // end class AI_Network


	// Global operators to allow sorting of networks based on their performance
	bool operator< (const Network &lhs, const Network &rhs);
	bool operator> (const Network &lhs, const Network &rhs);
	bool operator<= (const Network &lhs, const Network &rhs);
	bool operator>= (const Network &lhs, const Network &rhs);

	void setupNetworks(const std::vector<int> &);

	// Functions to handle saving and loading of networks utilizing boost serialization
	std::string idToFilename(int);
	void saveNetwork(int, Network &);
	bool loadNetwork(int, Network &);

}
#endif // NETWORK_H_INCLUDED