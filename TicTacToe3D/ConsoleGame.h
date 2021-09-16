#pragma once
#include <string>
#include "Game.h"
#include "Vec3.h"

class ConsoleGame
{
public:
	ConsoleGame();

private:
	bool game_ended_ = false;
	Game game_;
	void MainLoop();
	void DrawBoard();
	void DrawScore();
	std::string GetRowAsString(int z, int row);
	char GetCellCharAt(int x, int y, int z);
	Vec3 GetPosVec(char column, int row);
};
