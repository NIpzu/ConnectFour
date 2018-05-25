#pragma once
#include <vector>
#include <random>
#include "GameBoard.h"

class NeuralNetwork
{
public:
	NeuralNetwork(int numLayers, std::vector<int> numNeurons);
	std::vector<float> Execute(std::vector<float> inputs);
private:
	class Layer
	{
	public:
		Layer(std::mt19937& rng, int numNeurons, int lastNeurons);
		std::vector<float> Calculate(std::vector<float> in);
	private:
		class Neuron
		{
		public:
			Neuron(std::mt19937& rng, int lastNeurons);
			float GetValue(std::vector<float> in);
		private:
			std::vector<float> weights;
			float bias;
			float Sigmoid(float x) const;
		};
		std::vector<Neuron> neurons;
	};
	std::vector<Layer> layers;
	Layer outputLayer;
	static constexpr double e = 2.71828182845904523536028747135266249775724709369995;
	std::random_device rd;
	std::mt19937 rng;
};