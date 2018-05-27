#include "NeuroEvolver.h"

NeuroEvolver::NeuroEvolver(const size_t inputs, const size_t outputs)
	:
	inputs(inputs),
	outputs(outputs)
{
}

std::vector<NeuralNetwork> NeuroEvolver::nextGeneration()
{
	std::vector<NeuralNetwork> newGen;
	if (numCurGen == 0)
	{
		for (size_t i = 0; i < population; i++)
		{
			newGen.emplace_back(inputs, hiddenLayers, outputs,rng);
		}
		numCurGen++;
		return newGen;
	}
	else
	{
		for (size_t i = 0; i < size_t(elitism * float(population)); i++)
		{
			if (newGen.size() < population)
			{
				newGen.emplace_back(currentGeneration.genomes[i].network,rng);
			}
		}

		for (size_t i = 0; i < size_t(randomBehaviour * float(population)); i++)
		{
			if (newGen.size() < population)
			{
				newGen.emplace_back(inputs, hiddenLayers, outputs, rng);
			}
		}


		size_t max = 0;
		while (true)
		{
			for (size_t i = 0; i < max; i++)
			{
				auto children = Breed(currentGeneration.genomes[i].network, currentGeneration.genomes[max].network, (numChildren > 0 ? numChildren : 1));
				for (auto& child : children)
				{
					if (newGen.size() < population)
					{
						newGen.emplace_back(child, rng);
					}
					else
					{
						currentGeneration = Generation();
						numCurGen++;
						return std::move(newGen);
					}
				}
			}
			max++;
			if (max >= currentGeneration.genomes.size())
			{
				max = 0;
			}
		}

	}
}

std::vector<SavedNetwork> NeuroEvolver::Breed(const SavedNetwork & nn0, const SavedNetwork & nn1, const size_t numChildren)
{
	std::vector<SavedNetwork> children;
	for (size_t i = 0; i < numChildren; i++)
	{
		children.emplace_back();
		children[i].neuronsPerLayers = nn0.neuronsPerLayers;
		for (size_t j = 0; j < nn0.biases.size(); j++)
		{
			if (rng.GetRandom() < 0.5f)
			{
				children[i].biases.emplace_back(nn0.biases[j]);
			}
			else
			{
				children[i].biases.emplace_back(nn1.biases[j]);
			}
		}
		for (size_t j = 0; j < nn0.weights.size(); j++)
		{
			if (rng.GetRandom() < 0.5f)
			{
				children[i].weights.emplace_back(nn0.weights[j]);
			}
			else
			{
				children[i].weights.emplace_back(nn1.weights[j]);
			}
		}
		for (auto& weight : children[i].weights)
		{
			if (rng.GetRandom() <= mutationRate)
			{
				weight += rng.GetRandom() * 2 * mutationRange - mutationRange;
			}
		}
		for (auto& bias : children[i].biases)
		{
			if (rng.GetRandom() <= mutationRate)
			{
				bias += rng.GetRandom() * 2 * mutationRange - mutationRange;
			}
		}
	}
	return std::move(children);
}

void NeuroEvolver::ScoreNetwork(const NeuralNetwork network, const float score)
{
	if (currentGeneration.genomes.size() == 0)
	{
		Genome genome;
		genome.network = network.GetSave();
		genome.score = score;
		currentGeneration.genomes.emplace_back(genome);
		return;
	}

	for (auto i = currentGeneration.genomes.begin(); i < currentGeneration.genomes.end(); i++)
	{
		if (score > currentGeneration.genomes[i - currentGeneration.genomes.begin()].score)
		{
			Genome genome;
			genome.network = network.GetSave();
			genome.score = score;
			currentGeneration.genomes.insert(i, genome);
			return;
		}
	}

	Genome genome;
	genome.network = network.GetSave();
	genome.score = score;
	currentGeneration.genomes.insert(currentGeneration.genomes.end(), genome);
}
