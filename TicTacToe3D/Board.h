#pragma once
#include <vector>
#include <map>
#include "Cell.h"

class Board
{
private:
	static const int SIZE = 3;
	std::vector<std::vector<std::vector<Cell>>> board_;
	int possible_matches_;
public:
	Board();
	const Cell& GetCell(int x, int y, int z) const;
	bool SetCell(int x, int y, int z, char c);
	std::map<char, int> CheckMatches();
	bool IsInside(int x, int y, int z) const;
	bool CellHasSymbol(int x, int y, int z, char c) const;
	int GetNumberOfPossibleMatches() const;
};

