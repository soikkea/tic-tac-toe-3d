#pragma once
#include <string>

class ConsoleGame
{
public:
	ConsoleGame();

private:
	bool game_ended_ = false;
	void MainLoop();
	void DrawBoard();
	std::string GetRowAsString(int z, int row);
};

