#pragma once
#include "NeuralNetwork.h"

struct EvolvingSettings
{
	size_t inputs = 0;
	size_t outputs = 0;
	std::vector<size_t> hiddenLayers = {};
	size_t population = 100;
	size_t numChildren = 1;
	float mutationRate = 0.2f;
	float mutationRange = 0.5f;
	float elitism = 0.2f;
	float randomBehaviour = 0.2f;
};

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
	NeuroEvolver() = delete;
	NeuroEvolver(const EvolvingSettings& settings);
	std::vector<NeuralNetwork> nextGeneration();
	void ScoreNetwork(const NeuralNetwork network, const float score);
	Rng rng;
private:
	std::vector<SavedNetwork> Breed(const SavedNetwork& nn0, const SavedNetwork& nn1, const size_t numChildren);
	Generation currentGeneration;
	size_t numCurGen = 0;
	const EvolvingSettings settings;
};