#include "Board.h"
#include "Vec3.h"

Board::Board() : board_{{
	{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}}, 
	{{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}} , 
	{{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}, 
	{Cell(), Cell(), Cell()}} }, possible_matches_(1)
{
	CheckMatches();
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

std::map<char, int> Board::CheckMatches()
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
	auto cells = std::vector<const Cell*>(3);
	int valid_matches = 0;
	for (size_t z = 0; z < SIZE; z++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			for (size_t x = 0; x < SIZE; x++)
			{
				auto start_point = Vec3{ (int)x, (int)y, (int)z };
				cells[0] = (&GetCell(x, y, z));
				
				for(auto& direction : directions)
				{
					auto next_point = start_point + direction;
					if (!IsInside(next_point.x, next_point.y, next_point.z)) {
						continue;
					}
					cells[1] = &GetCell(next_point.x, next_point.y, next_point.z);
					next_point = next_point + direction;
					if (!IsInside(next_point.x, next_point.y, next_point.z)) {
						continue;
					}
					cells[2] = &GetCell(next_point.x, next_point.y, next_point.z);

					char c;
					auto symbol_found = false;
					int matches = 0;
					auto match_possible = true;
					for (auto cell : cells) {
						if (!symbol_found && !(cell->IsEmpty())) {
							c = cell->GetContent();
							symbol_found = true;
						}
						matches += (int)(symbol_found && c == cell->GetContent());

						match_possible = match_possible && ((symbol_found && c == cell->GetContent()) || cell->IsEmpty());
					}
					if (matches == 3) {
						if (map.count(c) == 0) {
							map[c] = 0;
						}
						map[c]++;
					}
					else {
						if (match_possible) {
							valid_matches++;
						}
					}

				}
			}
		}
	}
	possible_matches_ = valid_matches;
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

int Board::GetNumberOfPossibleMatches() const
{
	return possible_matches_;
}
