#include "Game.h"

Game::Game() : board_()
{
}

const Cell& Game::GetCell(int x, int y, int z) const
{
    return board_.GetCell(x, y, z);
}

bool Game::SetCell(int x, int y, int z, char c)
{
    return board_.SetCell(x, y, z, c);
}
