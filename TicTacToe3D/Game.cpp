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
    CountPoints();
}

const Player& Game::GetCurrentPlayer() const
{
    return players_[current_player_];
}

const std::vector<Player> Game::GetPlayers() const
{
    return players_;
}

void Game::CountPoints()
{
    auto points = board_.CheckMatches();
    for (auto& player : players_) {
        char symbol = player.GetSymbol();
        if (points.count(symbol) == 0) {
            player.SetPoints(0);
        }
        else {
            player.SetPoints(points[symbol]);
        }
    }
}

bool Game::PossibleMovesLeft() const
{
    return board_.GetNumberOfPossibleMatches() > 0;
}
