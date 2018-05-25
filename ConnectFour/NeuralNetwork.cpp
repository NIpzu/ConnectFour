#include "NeuralNetwork.h"
#include <cmath>

NeuralNetwork::Layer::Neuron::Neuron(std::mt19937 & rng,int lastNeurons)
{
	std::uniform_real_distribution<> dist(-1.0, 1.0);
	for (int i = 0; i < lastNeurons; i++)
	{
		weights.emplace_back(dist(rng));
	}
	bias = dist(rng);
}


float NeuralNetwork::Layer::Neuron::GetValue(std::vector<float> in)
{
	float value = 0.0f;
	for (size_t i = 0; i < weights.size(); i++)
	{
		value += in[i] * weights[i];
	}
	value += bias;
	value = Sigmoid(value);
	return value;
}

float NeuralNetwork::Layer::Neuron::Sigmoid(float x) const
{
	return 1 / (1 + (std::pow(e, -x)));
}

NeuralNetwork::Layer::Layer(std::mt19937 & rng, int numNeurons, int lastNeurons)
{
	for (int i = 0; i < numNeurons; i++)
	{
		neurons.emplace_back(rng, lastNeurons);
	}
}

std::vector<float> NeuralNetwork::Layer::Calculate(std::vector<float> in)
{
	std::vector<float> returnVal;
	for (size_t i = 0; i < neurons.size(); i++)
	{
		returnVal.emplace_back(neurons[i].GetValue(in));
	}
	return returnVal;
}

NeuralNetwork::NeuralNetwork(int numLayers, std::vector<int> numNeurons)
	:rng(rd()),
	outputLayer(rng, GameBoard::numColumns, numNeurons[numNeurons.size() - 1])
{
	if (numNeurons.size() != 0)
	{
		layers.emplace_back(rng, numNeurons[0], GameBoard::numColumns * GameBoard::numRows);
		for (size_t i = 1; i < numNeurons.size(); i++)
		{
			layers.emplace_back(rng, numNeurons[i], numNeurons[i - 1]);
		}
		outputLayer = Layer(rng, GameBoard::numColumns, numNeurons[numNeurons.size() - 1]);
	}
}

std::vector<float> NeuralNetwork::Execute(std::vector<float> inputs)
{
	for (size_t i = 0; i < layers.size(); i++)
	{
		inputs = layers[i].Calculate(inputs);
	}
	return outputLayer.Calculate(inputs);
}
