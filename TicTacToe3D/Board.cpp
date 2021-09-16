#include "Board.h"
#include "Vec3.h"

Board::Board() : board_{{{Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}}, {{Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}} , {{Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}, {Cell(), Cell(), Cell()}} }
{
}

const Cell& Board::GetCell(int x, int y, int z) const
{
	return board_[z][y][x];
}

bool Board::SetCell(int x, int y, int z, char c)
{
	auto& cell = board_[z][y][x];
	if (!cell.IsEmpty()) {
		return false;
	}
	cell.SetContent(c);
	return true;
}

std::map<char, int> Board::CheckMatches() const
{
	auto const directions = std::vector<Vec3>{
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		{-1, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 1, 1},
		{-1, 1, 1},
		{-1, 0, 1},
		{-1, -1, 1},
		{0, -1, 1},
		{1, -1, 1},
	};
	auto map = std::map<char, int>();
	for (size_t z = 0; z < SIZE; z++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			for (size_t x = 0; x < SIZE; x++)
			{
				auto start_point = Vec3{ (int)x, (int)y, (int)z };
				auto start_cell = GetCell(x, y, z);
				if (start_cell.IsEmpty()) {
					continue;
				}
				auto symbol = start_cell.GetContent();
				for(auto& direction : directions)
				{
					auto next_point = start_point + direction;
					if (CellHasSymbol(next_point.x, next_point.y, next_point.z, symbol)) {
						next_point = next_point + direction;
						if (CellHasSymbol(next_point.x, next_point.y, next_point.z, symbol)) {
							if (map.count(symbol) == 0) {
								map[symbol] = 0;
							}
							map[symbol]++;
						}
					}
				}
			}
		}
	}
	return map;
}

bool Board::IsInside(int x, int y, int z) const
{
	if ((x >= 0) && (x < SIZE) &&
		(y >= 0) && (y < SIZE) &&
		(z >= 0) && (z < SIZE)) {
		return true;
	}
	return false;
}

bool Board::CellHasSymbol(int x, int y, int z, char c) const
{
	if (!IsInside(x, y, z)) {
		return false;
	}
	auto cell = GetCell(x, y, z);
	return c == cell.GetContent();
}
