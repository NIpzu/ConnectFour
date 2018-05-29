#pragma once
#include <vector>
#include <random>
#include "GameBoard.h"

struct Neuron
{
	std::vector<float> weights;
	float bias;
	float value = 0.0f;
};

struct Layer
{
	std::vector<Neuron> neurons;
};

class Rng
{
public:
	Rng();
	float GetRandom();
	int GetRandomInt(const int min, const int max);
private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;
};

struct SavedNetwork
{
	std::vector<size_t> neuronsPerLayers;
	std::vector<float> weights;
	std::vector<float> biases;
};

class NeuralNetwork
{
public:
	NeuralNetwork(const size_t numInputs,const std::vector<size_t>& numHiddenNeurons, const size_t numOutputs, Rng& rng);
	NeuralNetwork(const SavedNetwork& savedNetwork, Rng& rng);
	void ConstructNextLayer(const size_t numNeurons);
	std::vector<float> Compute(const std::vector<float>& inputs);
	SavedNetwork GetSave() const;
	Rng& rng;
private:
	std::vector<Layer> layers;
	float Sigmoid(const float x) const;
};