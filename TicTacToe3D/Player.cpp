#include "Player.h"

Player::Player(char symbol) : symbol_(symbol)
{
}

char Player::GetSymbol() const
{
	return symbol_;
}
