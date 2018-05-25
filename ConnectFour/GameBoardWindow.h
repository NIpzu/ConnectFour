#pragma once
#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "NeuralNetwork.h"

class GameBoardWindow
{
public:
	GameBoardWindow();
	void Play();
private:
	void Update();
	void Draw();
	void AddPiece(int iColumn);
private:
	static constexpr int circleDiameter = 100;
	sf::Color player0color = sf::Color(255, 0, 0);
	sf::Color player1color = sf::Color(0, 0, 255);
	sf::Color emptycolor = sf::Color(0, 0, 0,255);
	sf::RenderWindow win;
	GameBoard gameboard;
	std::vector<sf::CircleShape> PieceSprites;
	NeuralNetwork * nn0 = new NeuralNetwork{ 6, {10,10,10,10,10,10} };
	NeuralNetwork * nn1 = new NeuralNetwork{ 6, {10,10,10,10,10,10} };
	long long int p0w = 0;
	long long int p1w = 0;
};
