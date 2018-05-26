#include "NeuralNetwork.h"
#include <cmath>
#include <assert.h>

NeuralNetwork::NeuralNetwork(const unsigned int numInputs, const std::vector<unsigned int>& numHiddenNeurons, const unsigned int numOutputs)
{
	//Input layer
	layers.emplace_back();
	for (unsigned int i = 0; i < numInputs; i++)
	{
		layers[layers.size() - 1].neurons.emplace_back();
	}

	//Hidden layers
	for (auto& numNeurons : numHiddenNeurons)
	{
		/*layers.emplace_back();
		for (unsigned int  i = 0; i < numNeurons; i++)
		{
			layers[layers.size() - 1].neurons.emplace_back();
			for (size_t j = 0; j < layers[layers.size() - 2].neurons.size(); j++)
			{
				layers[layers.size() - 1].neurons[i].weights.emplace_back(rng.GetRandom());
			}
			layers[layers.size() - 1].neurons[i].bias = rng.GetRandom();
		}*/
		ConstructNextLayer(numNeurons);
	}

	//Output layer
	/*layers.emplace_back();
	for (unsigned int i = 0; i < numOutputs; i++)
	{
		layers[layers.size() - 1].neurons.emplace_back();
		for (size_t j = 0; j < layers[layers.size() - 2].neurons.size(); j++)
		{
			layers[layers.size() - 1].neurons[i].weights.emplace_back(rng.GetRandom());
		}
		layers[layers.size() - 1].neurons[i].bias = rng.GetRandom();
	}*/
	ConstructNextLayer(numOutputs);
}

void NeuralNetwork::ConstructNextLayer(const unsigned int numNeurons)
{
	layers.emplace_back();
	for (unsigned int i = 0; i < numNeurons; i++)
	{
		layers[layers.size() - 1].neurons.emplace_back();
		for (size_t j = 0; j < layers[layers.size() - 2].neurons.size(); j++)
		{
			layers[layers.size() - 1].neurons[i].weights.emplace_back(rng.GetRandom());
		}
		layers[layers.size() - 1].neurons[i].bias = rng.GetRandom();
	}
}

std::vector<float> NeuralNetwork::Compute(const std::vector<float>& inputs)
{
	//Inputs
	if (inputs.size() != layers[0].neurons.size())
	{
		assert(false);
	}
	for (size_t i = 0; i < layers[0].neurons.size(); i++)
	{
		layers[0].neurons[i].value = inputs[i];
	}

	//Hidden layers + Output layer
	for (size_t i = 1; i < layers.size(); i++)
	{
		for (size_t j = 0; j < layers[i].neurons.size(); j++)
		{
			layers[i].neurons[j].value = 0.0f;
			for (size_t k = 0; k < layers[i].neurons[j].weights.size(); k++)
			{
				layers[i].neurons[j].value += layers[i - 1].neurons[k].value * layers[i].neurons[j].weights[k];
			}
			layers[i].neurons[j].value = Sigmoid(layers[i].neurons[j].value + layers[i].neurons[j].bias);
		}
	}
	//Calculate output
	std::vector<float> out;
	for (size_t i = 0; i < layers[layers.size() - 1].neurons.size(); i++)
	{
		out.emplace_back(layers[layers.size() - 1].neurons[i].value);
	}
	return std::move(out);
}

float NeuralNetwork::Sigmoid(const float x) const
{
	return 1.0f / (1.0f + (std::pow(2.7182818284590452353602874713526624977572470936999f, -x)));
}

Rng::Rng()
	: 
	rng(rd()),
	dist(-1.0f,1.0f)
{
}

float Rng::GetRandom()
{
	return dist(rng);
}

