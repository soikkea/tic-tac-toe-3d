#pragma once
#include <vector>
#include "Cell.h"
class Board
{
private:
	std::vector<std::vector<std::vector<Cell>>> board_;
public:
	Board();
	const Cell& GetCell(int x, int y, int z) const;
	bool SetCell(int x, int y, int z, char c);
};

