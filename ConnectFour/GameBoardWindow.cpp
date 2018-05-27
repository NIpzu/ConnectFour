#include "GameBoardWindow.h"

GameBoardWindow::GameBoardWindow()
	:
	win(sf::VideoMode(GameBoard::numColumns * circleDiameter, GameBoard::numRows * circleDiameter), "ConnectFour",sf::Style::Close | sf::Style::Titlebar,sf::ContextSettings(0,0,4))
{
	PieceSprites.resize(GameBoard::numColumns * GameBoard::numRows);
	for (size_t i = 0; i < PieceSprites.size(); i++)
	{
		PieceSprites[i].setFillColor(emptycolor);
		PieceSprites[i].setRadius(float(circleDiameter) / 2.0f);
		PieceSprites[i].setPosition(sf::Vector2f(float(i % GameBoard::numColumns * circleDiameter),float((GameBoard::numRows - 1) * circleDiameter - i / GameBoard::numColumns * circleDiameter)));
	}
}

void GameBoardWindow::Play()
{
	while (win.isOpen())
	{
		win.clear();
		Update();
		Draw();
		win.display();
	}
}

void GameBoardWindow::Update()
{
	sf::Event event;
	while (win.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			win.close();
	}
	
		/*if (gameboard.GetGameState() == GameState::player0turn)
		{
			auto& a = gameboard.GetGameBoard();
			std::vector<float> in;
			for (auto& p : a)
			{
				switch (p)
				{
				case Pieces::none:
					in.emplace_back(0.0f);
					break;
				case Pieces::player0:
					in.emplace_back(0.5f);
					break;
				case Pieces::player1:
					in.emplace_back(1.0f);
					break;
				default:
					break;
				}
			}
			auto out = nn0->Compute(in);
			float max = -1000.0f;
			int move;
			for (size_t i = 0; i < out.size(); i++)
			{
				if (out[i] > max)
				{
					move = (int)i;
					max = out[i];
				}
			}
			AddPiece(move);
		}
		else if (gameboard.GetGameState() == GameState::player1turn)
		{
			auto& a = gameboard.GetGameBoard();
			std::vector<float> in;
			for (auto& p : a)
			{
				switch (p)
				{
				case Pieces::none:
					in.emplace_back(0.0f);
					break;
				case Pieces::player0:
					in.emplace_back(0.5f);
					break;
				case Pieces::player1:
					in.emplace_back(1.0f);
					break;
				default:
					break;
				}
			}
			auto out = nn1->Compute(in);
			float max = -1000.0f;
			int move;
			for (size_t i = 0; i < out.size(); i++)
			{
				if (out[i] > max)
				{
					move = (int)i;
					max = out[i];
				}
			}
			AddPiece(move);

			
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Num1:
					AddPiece(0);
					break;
				case sf::Keyboard::Num2:
					AddPiece(1);
					break;
				case sf::Keyboard::Num3:
					AddPiece(2);
					break;
				case sf::Keyboard::Num4:
					AddPiece(3);
					break;
				case sf::Keyboard::Num5:
					AddPiece(4);
					break;
				case sf::Keyboard::Num6:
					AddPiece(5);
					break;
				case sf::Keyboard::Num7:
					AddPiece(6);
					break;
				default:
					break;
				}
			}
		}
		else if (gameboard.GetGameState() == GameState::player0wins)
		{
			gameboard = GameBoard();
		}
		else if (gameboard.GetGameState() == GameState::player1wins)
		{
			gameboard = GameBoard();
		}
		else
		{
			gameboard = GameBoard();
		}*/

		
	
}

void GameBoardWindow::Draw()
{
	for (auto& a : PieceSprites)
	{
		win.draw(a);
	}
}

void GameBoardWindow::AddPiece(int iColumn)
{
	gameboard.AddPiece(iColumn);
	const auto& pieces = gameboard.GetGameBoard();
	for (size_t i = 0; i < pieces.size(); i++)	
	{
		switch (pieces[i])
		{
		case Pieces::player0:
			PieceSprites[i].setFillColor(player0color);
			break;
		case Pieces::player1:
			PieceSprites[i].setFillColor(player1color);
			break;
		default:
			PieceSprites[i].setFillColor(emptycolor);
			break;
		}
	}
}
