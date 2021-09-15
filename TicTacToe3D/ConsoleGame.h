#pragma once
#include <string>
#include "Game.h"

struct Vec3
{
	int x;
	int y;
	int z;
};

class ConsoleGame
{
public:
	ConsoleGame();

private:
	bool game_ended_ = false;
	Game game_;
	void MainLoop();
	void DrawBoard();
	std::string GetRowAsString(int z, int row);
	char GetCellCharAt(int x, int y, int z);
	Vec3 GetPosVec(char column, int row);
};
