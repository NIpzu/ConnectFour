#include <algorithm>
#include <iostream>
#include "GameBoard.h"

GameBoard::GameBoard()
{
}

GameState GameBoard::AddPiece(int iColumn)
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
				return state;
			}
		}
		state = GameState::invalidMove;
		std::cout << "Invalid move!" << std::endl;
	}
	std::cout << "Can't move now!" << std::endl;
	return state;
}

const std::vector<Pieces> GameBoard::GetGameBoard() const
{
	return std::vector<Pieces>(&board[0],&board[numColumns * numRows -1]);
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

void GameBoard::CheckForVictory(int iOrigin)
{
	const Pieces checkForPiece = board[iOrigin];
	int originY = iOrigin / numColumns;
	int originX = iOrigin % numColumns;

	int xCount = 1;
	if (originX != numColumns)
	{

		for (int x = originX + 1; x < std::min(originX + 4, numColumns); x++)
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
	}
	if (originX != 0)
	{

		for (int x = originX - 1; x >= std::max(originX - 4, 0); x--)
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
	}
	if (xCount >= 4)
	{
		state = Win();
		std::cout << "Player " << (state == GameState::player0wins ? "0" : "1") << " wins!" << std::endl;
	}


}
