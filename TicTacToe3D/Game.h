#pragma once
#include <vector>
#include "Board.h"
#include "Cell.h"
#include "Player.h"

class Game
{
private:
	Board board_;
	std::vector<Player> players_;
	int current_player_;
public:
	Game();
	const Cell& GetCell(int x, int y, int z) const;
	bool SetCell(int x, int y, int z, char c);
	void EndTurn();
	const Player& GetCurrentPlayer() const;
	const std::vector<Player> GetPlayers() const;
	void CountPoints();
};

