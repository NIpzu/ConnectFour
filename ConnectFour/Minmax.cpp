#include "Minmax.h"
#include <algorithm>


float MinMaxNode::GetNodeValue(const int depth, const bool maximizingPlayer) const
{
	if (depth == 0 || IsTerminal())
	{
		return GetValue();
	}
	if (maximizingPlayer)
	{
		float bestValue = -1.0f;
		for (auto& child : GetChildren())
		{
			const float v = child.GetNodeValue(depth - 1, false);
			bestValue = std::max(v, bestValue);
		}
		return bestValue;
	}
	else
	{
		float bestValue = 1.0f;
		for (auto& child : GetChildren())
		{
			const float v = child.GetNodeValue(depth - 1, true);
			bestValue = std::min(v, bestValue);
		}
		return bestValue;
	}
}

ConnectFourNode::ConnectFourNode(const GameBoard & board)
	:
	board(board)
{
}

std::vector<MinMaxNode> ConnectFourNode::GetChildren() const
{
	std::vector<ConnectFourNode> out;
	for (size_t i = 0; i < GameBoard::numColumns; i++)
	{
		auto child = board;
		child.AddPiece(i);
		ConnectFourNode childNode(child);
		out.emplace_back(std::move(childNode));
	}
	return out;
}

bool ConnectFourNode::IsTerminal() const
{
	return !(board.GetGameState() == GameState::player0turn || board.GetGameState() == GameState::player1turn);
}

float ConnectFourNode::GetValue() const
{
	return 0.0f; //TODO
}
