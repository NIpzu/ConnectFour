#include "GameBoardWindow.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <thread>
#include "Minmax.h"

void MakeMoveNN(NeuralNetwork nn, GameBoard& gb)
{
	std::vector<float> inputs;
	std::vector<float> outputs;
	for (const auto& val : gb.GetGameBoard())
	{
		if (gb.GetGameState() == GameState::player0turn)
		{
			inputs.emplace_back(val == Pieces::none ? 0.0f : (val == Pieces::player1 ? -1.0f : 1.0f));
		}
		if (gb.GetGameState() == GameState::player1turn)
		{
			inputs.emplace_back(val == Pieces::none ? 0.0f : (val == Pieces::player0 ? -1.0f : 1.0f));
		}
	}
	outputs = nn.Compute(inputs);
	gb.AddPiece(int(outputs.begin() - std::max_element(outputs.begin(), outputs.end())));
}

void MakeMoveMM(GameBoard& gb, const int depth, const bool player0, Rng& rng)
{
	ConnectFourNode base = gb;
	auto children = base.GetChildren();
	int bestVal = std::round(children[0]->GetNodeValue(depth, !player0));

	std::vector<size_t> iBestValues = { 0 };
	for (size_t i = 1; i < children.size(); i++)
	{
		float Val = children[i]->GetNodeValue(depth, !player0);
		if (!player0)
		{
			if (bestVal > Val)
			{
				bestVal = Val;
				iBestValues = { i };
			}
			else if (bestVal == Val)
			{
				iBestValues.emplace_back(i);
			}
		}
		else
		{
			if (bestVal < Val)
			{
				bestVal = Val;
				iBestValues = { i };
			}
			else if (bestVal == Val)
			{
				iBestValues.emplace_back(i);
			}
		}
	}
	int index = rng.GetRandomInt(0, iBestValues.size() - 1);
	gb.AddPiece(iBestValues[index]);
}

int PlayMatchNN(NeuralNetwork nn0, NeuralNetwork nn1)
{
	GameBoard gb;
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
			MakeMoveNN(nn0, gb);
			break;
		case GameState::player1turn:
			MakeMoveNN(nn1, gb);
			break;
		default:
			break;
		}
	}
}

float PlayMatchMM(NeuralNetwork nn, bool nnIsPlayer0)
{
	GameBoard gb;
	float score = 0.0f;
	while (true)
	{
		switch (gb.GetGameState())
		{
		case GameState::draw:
			return 100.0f - score;
		case GameState::player0wins:
			return (nnIsPlayer0 ? 500.0f - score : score);
		case GameState::player1wins:
			return (!nnIsPlayer0 ? 500.0f - score: score);
		case GameState::player0turn:
			if (nnIsPlayer0)
			{
				MakeMoveNN(nn, gb);
			}
			else
			{
				MakeMoveMM(gb, 2, true, nn.rng);
			}
			score++;
			break;
		case GameState::player1turn:
			if (!nnIsPlayer0)
			{
				MakeMoveNN(nn, gb);
			}
			else
			{
				MakeMoveMM(gb, 2, false, nn.rng);
			}
			score++;
			break;
		default:
			break;
		}
	}
}

void PlayGamesNN(std::vector<NeuralNetwork>& networks, std::vector<float>& networkScores, const size_t begin, const size_t end)
{
	for (size_t i = begin; i < end; i++)
	{
		for (size_t j = 0; j < networks.size(); j++)
		{
			int result = PlayMatchNN(networks[i], networks[j]);
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

void PlayGamesMM(std::vector<NeuralNetwork>& networks, std::vector<float>& networkScores, const size_t begin, const size_t end)
{
	for (size_t i = begin; i < end; i++)
	{
		float result = PlayMatchMM(networks[i], true);
		result += PlayMatchMM(networks[i], false);
		networkScores[i] += float(result);
	}
}

int main()
{
	
	EvolvingSettings settings;

	settings.inputs = GameBoard::numColumns * GameBoard::numRows;
	settings.outputs = GameBoard::numColumns;
	settings.hiddenLayers = { 100 };
	NeuroEvolver evolver(settings);
	std::vector<NeuralNetwork> networks;
	std::vector<float> networkScores;

	int generations = 20;

	unsigned int numThreads = std::thread::hardware_concurrency();

	if ( false|| numThreads == 0)
	{
		numThreads = 1;
	}

	std::vector<std::thread> threads;

	for (size_t g = 0; g < generations; g++)
	{
		std::cout << "Training generation " << g + 1 << std::endl;
		networks = evolver.nextGeneration();
		networkScores.clear();
		networkScores.resize(networks.size());
		for (auto& s : networkScores)
		{
			s = 0.0f;
		}
		if (numThreads != 1)
		{
			for (unsigned int j = 0; j < numThreads - 1; j++)
			{
				threads.emplace_back(&PlayGamesMM, std::ref(networks), std::ref(networkScores), networks.size() / numThreads * j, networks.size() / numThreads * (j + 1));
			}
		}
		threads.emplace_back(&PlayGamesMM, std::ref(networks), std::ref(networkScores), networks.size() / numThreads * (numThreads - 1), networks.size());

		for (auto& t : threads)
		{
			t.join();
		}
		threads.clear();

		for (size_t i = 0; i < networks.size(); i++)
		{
			evolver.ScoreNetwork(networks[i], networkScores[i]);
		}
		std::cout << networkScores[std::max_element(networkScores.begin(), networkScores.end()) - networkScores.begin()] << std::endl;
		//GameBoardWindow gbw;
		//gbw.Play(networks[std::max_element(networkScores.begin(), networkScores.end()) - networkScores.begin()]);
	}

	GameBoardWindow gbw;
	gbw.Play(evolver.nextGeneration()[0]);


	return 0;
}