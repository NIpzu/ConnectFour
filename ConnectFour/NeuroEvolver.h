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
	void ScoreNetwork(const NeuralNetwork network, const float score);
	Rng rng;
private:
	std::vector<SavedNetwork> Breed(const SavedNetwork& nn0, const SavedNetwork& nn1, const size_t numChildren);
	Generation currentGeneration;
	float maxScore;
	size_t numCurGen = 0;
	const size_t inputs, outputs;
	size_t population = 100;
	size_t numChildren = 1;
	std::vector<size_t> hiddenLayers = {50};
	float mutationRate = 0.2f;
	float mutationRange = 0.5f;
	float elitism = 0.2f;
	float randomBehaviour = 0.2f;
};