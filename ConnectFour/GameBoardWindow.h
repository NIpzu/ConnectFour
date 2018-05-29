#pragma once
#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "NeuroEvolver.h"

class GameBoardWindow
{
public:
	GameBoardWindow();
	void Play(NeuralNetwork& nn);
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
	NeuralNetwork * enemy;
	Rng rng;
};
