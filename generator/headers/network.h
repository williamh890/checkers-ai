#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <vector>
using std::vector;

class AI_Network {
public:
	using weightsInLayer = vector<double>;

	AI_Network(int);
	AI_Network(const vector<int>&);
	~AI_Network();

	void setupNetwork();
	void setupNetwork(const vector<int> &);

	void loadWeightsFromFile(); // *** TODO ***
	void saveWeightsToFile();	// *** TODO ***
	
	double evaluateBoard(vector<char>) const; // *** TODO ***
	void adjustPerformance(int result);
	int getPerformance() const;
	void resetPerformance();

	vector<weightsInLayer> & evolve() const; // *** TODO ***
	void replaceWithEvolution(vector<weightsInLayer> &);

private:
	int _ID;
	vector<weightsInLayer> _layers;
	int _performance;
	bool _gameCompleted = false;
}; // end class AI_Network

bool operator< (const AI_Network &lhs, const AI_Network &rhs);
bool operator> (const AI_Network &lhs, const AI_Network &rhs);
bool operator<= (const AI_Network &lhs, const AI_Network &rhs);
bool operator>= (const AI_Network &lhs, const AI_Network &rhs);

#endif // NETWORK_H_INCLUDED