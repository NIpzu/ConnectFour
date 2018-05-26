#pragma once
#include "NeuralNetwork.h"

struct Genome
{
	float score;
	SavedNetwork network;
};

struct Generation
{
	std::vector<Genome> genomes;
};

class NeuroEvolver
{
public:
	NeuroEvolver(const size_t inputs, const size_t outputs);
	std::vector<NeuralNetwork> nextGeneration();
	std::vector<SavedNetwork> Breed(const SavedNetwork& nn0, const SavedNetwork& nn1, const size_t numChildren);
	void ScoreNetwork(const NeuralNetwork network, const float score);
private:
	Generation currentGeneration;
	float maxScore;
	Rng rng;
	size_t numCurGen = 0;
	const size_t inputs, outputs;
	size_t population = 50;
	size_t numChildren = 1;
	std::vector<size_t> hiddenLayers = {5,5,5};
	float mutationRate = 0.2f;
	float mutationRange = 0.5f;
	float elitism = 0.2f;
	float randomBehaviour = 0.2f;
};