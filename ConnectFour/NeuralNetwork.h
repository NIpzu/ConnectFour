#pragma once
#include <vector>
#include <random>
#include "GameBoard.h"

struct Neuron
{
	float value;
	std::vector<float> weights;
	float bias;
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
private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;
};

class NeuralNetwork
{
public:
	NeuralNetwork(const unsigned int numInputs,const std::vector<unsigned int>& numHiddenNeurons, const unsigned int  numOutputs);
	void ConstructNextLayer(const unsigned int numNeurons);
	std::vector<float> Compute(const std::vector<float>& inputs);
private:
	std::vector<Layer> layers;
	float Sigmoid(const float x) const;
	Rng rng;
};