#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <vector>

namespace ai {
    class Network {
        public:
            using weightsInLayer = std::vector<double>;

            Network(int);
            Network(const std::vector<int>&);
            ~Network();

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
    }; // end class Network

    bool operator< (const Network &lhs, const Network &rhs);
    bool operator> (const Network &lhs, const Network &rhs);
    bool operator<= (const Network &lhs, const Network &rhs);
    bool operator>= (const Network &lhs, const Network &rhs);
}

#endif // NETWORK_H_INCLUDED
