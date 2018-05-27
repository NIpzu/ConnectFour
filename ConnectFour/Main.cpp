#include "GameBoardWindow.h"
#include "NeuralNetwork.h"
#include <iostream>


int main()
{

	NeuroEvolver evolver(GameBoard::numColumns * GameBoard::numRows,GameBoard::numColumns);
	std::vector<NeuralNetwork> networks;
	std::vector<float> networkScores;

	if (networks.size() == 0)
	{
		networks = evolver.nextGeneration();
	}
	else
	{
		size_t iPair0 = evolver.rng.GetRandomInt(0, networks.size() - 1);
		NeuralNetwork pair0 = networks[iPair0];
		networks.erase(networks.begin() + iPair0);

		size_t iPair1 = evolver.rng.GetRandomInt(0, networks.size() - 1);
		NeuralNetwork pair1 = networks[iPair1];
		networks.erase(networks.begin() + iPair1);


	}


	return 0;
}