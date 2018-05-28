#pragma once
#include <vector>
#include "GameBoard.h"
#include <memory>

class MinMaxNode
{
public:
	virtual std::vector<std::unique_ptr<MinMaxNode>> GetChildren() const = 0;
	virtual bool IsTerminal() const = 0;
	virtual float GetValue() const = 0;
	float GetNodeValue(const int depth, const bool maximizingPlayer) const;
};



class ConnectFourNode : MinMaxNode
{
public:
	ConnectFourNode(const GameBoard& board);
	std::vector<std::unique_ptr<MinMaxNode>> GetChildren() const override;
private:
	bool IsTerminal() const override;
	float GetValue() const override;
	GameBoard board;
};
