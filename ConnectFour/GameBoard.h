#pragma once
#include <vector>

enum class GameState
{
	player0wins,
	player1wins,
	player0turn,
	player1turn,
	draw
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
	void AddPiece(const int iColumn);
	const std::vector<Pieces> GetGameBoard() const;
	GameState GetGameState() const;
private:
	void ChangeTurn();
	GameState Win() const;
	GameState Lose() const;
	Pieces CurrentPlayer() const;
	void CheckForVictory(const int iOrigin);
private:
	Pieces board[numRows * numColumns] = { Pieces::none };
	GameState state = GameState::player0turn;
};