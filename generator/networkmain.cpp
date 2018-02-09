// main cpp that can be copied and pasted with minor changes to allow users to test implementation
// of boost libraries on their system outside of the entire ai project.

//Change this include to your own path to the network.h
//network.h looks for things locally and so it should be fine to run unaltered.
#include "C:\Users\Duane\Documents\dlshaffer\checkers-ai\generator\headers\network.h"
#include <iostream>
#include <chrono>
#include <vector>
using std::vector;

double get_time() {
	return 1.0e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
		).count();
}

int main() {
		vector <unsigned int> networkCreationInput = { 4, 32, 40, 10, 1 };


		// Create only one network so that you dont have 100 files running around. Leave off the second parameter defaults to 100
		AI::setupNetworks(networkCreationInput, 1);


		AI::Network player(1);
		//Proof of success at runtime
		//player.outputDebug();

		// Wait for user
		std::cin.ignore();
		std::cout << "Press ENTER to quit ";
		while (std::cin.get() != '\n');


	return 0;
}