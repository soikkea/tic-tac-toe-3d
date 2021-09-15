#include "Game.h"

Game::Game() : board_(), players_{Player('X'), Player('O')}, current_player_(0)
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

void Game::EndTurn()
{
    current_player_ = (current_player_ + 1) % players_.size();
}

const Player& Game::GetCurrentPlayer() const
{
    return players_[current_player_];
}
