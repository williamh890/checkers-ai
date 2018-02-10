#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <vector>

// You need to tell your system how to find the boost library
#include <boost\archive\basic_text_iarchive.hpp>
#include <boost\archive\basic_text_oarchive.hpp>
#include <boost\serialization\vector.hpp>
#include <fstream>
#include <iostream>
#include <string>
namespace AI {
	class Network {
		bool DEBUG = true;
	public:
		using layersContainingNodes = std::vector<double>;
		using networkWeights = std::vector<double>;

		Network(unsigned int);
		Network(const std::vector<unsigned int>&, unsigned int); // This should probably be made private
		~Network();

		double evaluateBoard (const std::vector<char> &); // *** TODO ***
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

		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive &ar, const unsigned int version) {
			ar & _ID;
			ar & _layers;
			ar & _weights;
			ar & _kingWeight;
			ar & _performance;
		}

		void calculateNode(unsigned int, unsigned int);
	}; // end class AI_Network


	// Global operators to allow sorting of networks based on their performance
	bool operator< (const Network &lhs, const Network &rhs);
	bool operator> (const Network &lhs, const Network &rhs);
	bool operator<= (const Network &lhs, const Network &rhs);
	bool operator>= (const Network &lhs, const Network &rhs);

	void setupNetworks (const std::vector<unsigned int> &, int num = 100);

	// Functions to handle saving and loading of networks utilizing boost serialization
	std::string idToFilename(int);
	void saveNetwork(int, Network &);
	bool loadNetwork(int, Network &);

}
#endif // NETWORK_H_INCLUDED