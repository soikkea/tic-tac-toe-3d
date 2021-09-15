#include "Board.h"

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
