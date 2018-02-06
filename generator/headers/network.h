#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <vector>

class AI_Network {
public:
	using weightsInLayer = std::vector<double>;

	AI_Network(int);
	AI_Network(const std::vector<int>&);
	~AI_Network();

	void setupNetwork();
	void setupNetwork(const std::vector<int> &);

	void loadWeightsFromFile(); // *** TODO ***
	void saveWeightsToFile();	// *** TODO ***
	
	double evaluateBoard(std::vector<char>) const; // *** TODO ***
	void adjustPerformance(int result);
	int getPerformance() const;
	void resetPerformance();

	std::vector<weightsInLayer> & evolve() const; // *** TODO ***
	void replaceWithEvolution(std::vector<weightsInLayer> &);

private:
	int _ID;
	std::vector<weightsInLayer> _layers;
	int _performance;
	bool _gameCompleted = false;
}; // end class AI_Network

bool operator< (const AI_Network &lhs, const AI_Network &rhs);
bool operator> (const AI_Network &lhs, const AI_Network &rhs);
bool operator<= (const AI_Network &lhs, const AI_Network &rhs);
bool operator>= (const AI_Network &lhs, const AI_Network &rhs);

#endif // NETWORK_H_INCLUDED