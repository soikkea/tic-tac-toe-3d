#pragma once
class Player
{
private:
	char symbol_;
	int points_;
public:
	Player(char symbol);
	char GetSymbol() const;
	int GetPoints() const;
	void SetPoints(int points);
};

