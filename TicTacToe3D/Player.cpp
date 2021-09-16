#include "Player.h"

Player::Player(char symbol) : symbol_(symbol), points_(0)
{
}

char Player::GetSymbol() const
{
	return symbol_;
}

int Player::GetPoints() const
{
	return points_;
}

void Player::SetPoints(int points)
{
	points_ = points;
}
