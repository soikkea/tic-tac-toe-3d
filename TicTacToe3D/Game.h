#pragma once
#include "Board.h"
#include "Cell.h"
class Game
{
private:
	Board board_;
public:
	Game();
	const Cell& GetCell(int x, int y, int z) const;
	bool SetCell(int x, int y, int z, char c);
};

