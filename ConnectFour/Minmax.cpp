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
		float bestValue = -100.0f;
		std::vector<std::unique_ptr<MinMaxNode>> children = GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			const float v = children[i]->GetNodeValue(depth - 1, false);
			bestValue = std::max(v, bestValue);
		}
		return bestValue;
	}
	else
	{
		float bestValue = 100.0f;
		std::vector<std::unique_ptr<MinMaxNode>> children = GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			const float v = children[i]->GetNodeValue(depth - 1, true);
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

std::vector<std::unique_ptr<MinMaxNode>> ConnectFourNode::GetChildren() const
{
	std::vector<std::unique_ptr<MinMaxNode>> out;
	for (int i = 0; i < GameBoard::numColumns; i++)
	{
		auto child = board;
		child.AddPiece(i);
		std::unique_ptr<MinMaxNode> pChildNode(new ConnectFourNode(child));
		out.push_back(std::move(pChildNode));
	}
	return std::move(out);
}

bool ConnectFourNode::IsTerminal() const
{
	return !(board.GetGameState() == GameState::player0turn || board.GetGameState() == GameState::player1turn || board.GetGameState() == GameState::draw);
}

float ConnectFourNode::GetValue() const
{
	switch (board.GetGameState())
	{
	case GameState::draw:
		return 0.0f;
	case GameState::player0wins:
		return 1.0f;
	case GameState::player1wins:
		return -1.0f;
	default:
		return 0.0f;
	}
}
