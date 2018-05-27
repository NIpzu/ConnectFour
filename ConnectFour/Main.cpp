#include "GameBoardWindow.h"
#include "NeuralNetwork.h"
#include <iostream>

int PlayMatch(NeuralNetwork& nn0, NeuralNetwork& nn1)
{
	GameBoard gb;
	std::vector<float> inputs;
	while (true)
	{
		switch (gb.GetGameState())
		{
		case GameState::draw:
			return 0;
		case GameState::player0wins:
			return 1;
		case GameState::player1wins:
			return -1;
		case GameState::player0turn:
			inputs.clear();
			for (const auto& val : gb.GetGameBoard())
			{
				inputs.emplace_back(val == Pieces::none ?  0.0f : (val == Pieces::player0 ? 0.5f : 1.0f));
			}
			nn0.Compute(inputs);
			if (gb.GetGameState() == GameState::invalidMove)
				return -1;
			break;
		case GameState::player1turn:
			inputs.clear();
			for (const auto& val : gb.GetGameBoard())
			{
				inputs.emplace_back(val == Pieces::none ? 0.0f : (val == Pieces::player1 ? 0.5f : 1.0f));
			}
			nn1.Compute(inputs);
			if (gb.GetGameState() == GameState::invalidMove)
				return 1;
			break;
		default:
			break;
		}
	}
}

int main()
{

	NeuroEvolver evolver(GameBoard::numColumns * GameBoard::numRows,GameBoard::numColumns);
	std::vector<NeuralNetwork> networks;
	std::vector<float> networkScores;

	if (networks.size() == 0)
	{
		networks = evolver.nextGeneration();
		networkScores.resize(networks.size());
	}
	else
	{
		for (size_t i = 0; i < networks.size(); i++)
		{
			for (size_t j = i; j < networks.size(); j++)
			{
				const int result = PlayMatch(networks[i], networks[j]);
				if (result == -1)
				{
					networkScores[j] += 1.0f;
					networkScores[i] -= 1.0f;
				}
				if (result == 1)
				{
					networkScores[i] += 1.0f;
					networkScores[j] -= 1.0f;
				}
			}
		}
		for (size_t i = 0; i < networks.size(); i++)
		{
			evolver.ScoreNetwork(networks[i], networkScores[i]);
		}
	}


	return 0;
}