#pragma once
#include <vector>

enum class GameState
{
	player0wins,
	player1wins,
	player0turn,
	player1turn,
	draw,
	invalidMove
};

enum class Pieces
{
	none,
	player0,
	player1
};

class GameBoard
{
public:
	static constexpr int numRows = 6;
	static constexpr int numColumns = 7;
	GameBoard();
	GameState AddPiece(int iColumn);
	const std::vector<Pieces> GetGameBoard() const;
private:
	void ChangeTurn();
	GameState Win() const;
	Pieces CurrentPlayer() const;
	bool CheckForVictory(int iOrigin);
private:
	Pieces board[numRows * numColumns] = { Pieces::none };
	GameState state = GameState::player0turn;
};