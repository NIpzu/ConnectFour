#include "GameBoardWindow.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <thread>

int PlayMatch(NeuralNetwork nn0, NeuralNetwork nn1)
{
	GameBoard gb;
	std::vector<float> inputs;
	std::vector<float> outputs;
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
				inputs.emplace_back(val == Pieces::none ?  0.0f : (val == Pieces::player0 ? -1.0f : 1.0f));
			}
			outputs = nn0.Compute(inputs);
			gb.AddPiece(outputs.begin() - std::max_element(outputs.begin(), outputs.end()));
			if (gb.GetGameState() == GameState::invalidMove)
				return -1;
			break;
		case GameState::player1turn:
			inputs.clear();
			for (const auto& val : gb.GetGameBoard())
			{
				inputs.emplace_back(val == Pieces::none ? 0.0f : (val == Pieces::player1 ? -1.0f : 1.0f));
			}
			outputs = nn1.Compute(inputs);
			gb.AddPiece(std::max_element(outputs.begin(), outputs.end()) - outputs.begin());
			if (gb.GetGameState() == GameState::invalidMove)
				return 1;
			break;
		default:
			break;
		}
	}
}

void PlayGames(std::vector<NeuralNetwork>& networks, std::vector<float>& networkScores, const size_t begin, const size_t end)
{
	for (size_t i = begin; i < end; i++)
	{
		for (size_t j = 0; j < networks.size(); j++)
		{
			int result = PlayMatch(networks[i], networks[j]);
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
}

int main()
{

	NeuroEvolver evolver(GameBoard::numColumns * GameBoard::numRows, GameBoard::numColumns);
	std::vector<NeuralNetwork> networks;
	std::vector<float> networkScores;

	int generations = 1000;

	unsigned int numThreads = std::thread::hardware_concurrency();

	if (numThreads == 0)
	{
		numThreads = 1;
	}

	std::vector<std::thread> threads;

	for (size_t g = 0; g < generations; g++)
	{
		std::cout << "Training gen " << g + 1 << std::endl;
		//if (networks.size() == 0)
		//{
		networks = evolver.nextGeneration();
		networkScores.clear();
		networkScores.resize(networks.size());
		for (auto& s : networkScores)
		{
			s = 0.0f;
		}
		//}
		//else
		//{
		if (numThreads != 1)
		{
			for (unsigned int j = 0; j < numThreads - 1; j++)
			{
				threads.emplace_back(&PlayGames, std::ref(networks), std::ref(networkScores), networks.size() / numThreads * j, networks.size() / numThreads * (j + 1));
			}
		}
		threads.emplace_back(&PlayGames, std::ref(networks), std::ref(networkScores), networks.size() / numThreads * (numThreads - 1), networks.size());

		for (auto& t : threads)
		{
			t.join();
		}
		threads.clear();
		/*for (size_t j = i + 1; j < networks.size(); j++)
		{
			int result = PlayMatch(networks[i], networks[j]);
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
			result = PlayMatch(networks[j], networks[i]);
			if (result == -1)
			{
				networkScores[i] += 1.0f;
				networkScores[j] -= 1.0f;
			}
			if (result == 1)
			{
				networkScores[j] += 1.0f;
				networkScores[i] -= 1.0f;
			}
		}*/

		for (size_t i = 0; i < networks.size(); i++)
		{
			evolver.ScoreNetwork(networks[i], networkScores[i]);
		}
		//}
	}

	GameBoardWindow gbw;
	gbw.Play(evolver.nextGeneration()[0]);


	return 0;
}