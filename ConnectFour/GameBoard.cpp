#include <algorithm>
#include <iostream>
#include "GameBoard.h"

void GameBoard::AddPiece(const int iColumn)
{
	if (CurrentPlayer() != Pieces::none)
	{
		for (int i = 0; i < numRows; i++)
		{
			if (board[i * numColumns + iColumn] == Pieces::none)
			{
				board[i * numColumns + iColumn] = CurrentPlayer();
				CheckForVictory(i * numColumns + iColumn);
				ChangeTurn();
				return;
			}
		}
		state = GameState::invalidMove;
		//std::cout << "Invalid move!" << std::endl;
	}
	//std::cout << "Can't move now!" << std::endl;
}

const std::vector<Pieces> GameBoard::GetGameBoard() const
{
	return std::vector<Pieces>(&board[0],&board[numColumns * numRows]);
}

GameState GameBoard::GetGameState() const
{
	return state;
}

void GameBoard::ChangeTurn()
{
	switch (state)
	{
	case GameState::player0turn:
		state = GameState::player1turn;
		break;
	case GameState::player1turn:
		state = GameState::player0turn;
		break;
	default:
		break;
	}
}

GameState GameBoard::Win() const
{
	switch (state)
	{
	case GameState::player0turn:
		return GameState::player0wins;
		break;
	case GameState::player1turn:
		return GameState::player1wins;
		break;
	default:
		return state;
		break;
	}
}

Pieces GameBoard::CurrentPlayer() const
{
	switch (state)
	{
	case GameState::player0turn:
		return Pieces::player0;
		break;
	case GameState::player1turn:
		return Pieces::player1;
		break;
	default:
		return Pieces::none;
		break;
	}
}

void GameBoard::CheckForVictory(const int iOrigin)
{
	if (std::any_of(std::begin(board), std::end(board), [](Pieces p) {return p == Pieces::none; }))
	{
		const Pieces checkForPiece = board[iOrigin];
		int originY = iOrigin / numColumns;
		int originX = iOrigin % numColumns;

		int maxLeft = std::max(originX - 4, 0);
		int maxDown = std::max(originY - 4, 0);
		int maxRight = std::min(originX + 4, numColumns);
		int maxUp = std::min(originY + 4, numRows);

		int maxLeftMov = std::min(originX, 3);
		int maxDownMov = std::min(originY, 3);
		int maxRightMov = std::min(numColumns - originX, 3);
		int maxUpMov = std::min(numRows - originY, 3);

		int xCount = 1;

		for (int x = originX + 1; x < maxRight; x++)
		{
			if (board[originY * numColumns + x] == checkForPiece)
			{
				xCount++;
			}
			else
			{
				break;
			}
		}


		for (int x = originX - 1; x >= maxLeft; x--)
		{
			if (board[originY * numColumns + x] == checkForPiece)
			{
				xCount++;
			}
			else
			{
				break;
			}
		}

		if (xCount >= 4)
		{
			state = Win();
			//std::cout << "Player " << (state == GameState::player0wins ? "0" : "1") << " wins!" << std::endl;
		}



		int yCount = 1;

		for (int y = originY + 1; y < maxUp; y++)
		{
			if (board[y * numColumns + originX] == checkForPiece)
			{
				yCount++;
			}
			else
			{
				break;
			}
		}

		for (int y = originY - 1; y >= maxDown; y--)
		{
			if (board[y * numColumns + originX] == checkForPiece)
			{
				yCount++;
			}
			else
			{
				break;
			}
		}

		if (yCount >= 4)
		{
			state = Win();
			//std::cout << "Player " << (state == GameState::player0wins ? "0" : "1") << " wins!" << std::endl;
		}

		int xyCount = 0;
		for (int xy = 0; xy < std::min(maxRightMov, maxUpMov); xy++)
		{
			if (board[(originY + xy) * numColumns + originX + xy] == checkForPiece)
			{
				xyCount++;
			}
			else
			{
				break;
			}
		}

		for (int xy = 0; xy >= std::max(maxLeftMov, maxDownMov); xy--)
		{
			if (board[(originY + xy) * numColumns + originX + xy] == checkForPiece)
			{
				xyCount++;
			}
			else
			{
				break;
			}
		}

		if (xyCount >= 4)
		{
			state = Win();
		}



		xyCount = 0;
		for (int xy = 0; xy < std::min(maxRightMov, maxDownMov); xy++)
		{
			if (board[(originY - xy) * numColumns + originX + xy] == checkForPiece)
			{
				xyCount++;
			}
			else
			{
				break;
			}
		}

		for (int xy = 0; xy >= std::max(maxLeftMov, maxUpMov); xy--)
		{
			if (board[(originY - xy) * numColumns + originX + xy] == checkForPiece)
			{
				xyCount++;
			}
			else
			{
				break;
			}
		}

		if (xyCount >= 4)
		{
			state = Win();
		}
	}
	else
	{
		state = GameState::draw;
	}
}
