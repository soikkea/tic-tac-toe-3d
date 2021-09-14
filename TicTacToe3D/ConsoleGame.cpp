#include "ConsoleGame.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

ConsoleGame::ConsoleGame()
{
	MainLoop();
}

void ConsoleGame::MainLoop()
{
	while (!game_ended_) {
		DrawBoard();
		std::cout << "Type \"Quit\" to quit: ";
		std::string command;
		std::cin >> command;
		std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {return std::tolower(c); });

		if (command == "quit" || command == "q") {
			game_ended_ = true;
		}
		else {
			std::cout << "Unrecognized command!\n";
		}
	}
}

void ConsoleGame::DrawBoard()
{
	std::cout << "   A B C   D E F   G H I\n";
	for (size_t i = 0; i < 3; i++)
	{
		std::cout << " " << i + 1 << GetRowAsString(0, i) << " " << GetRowAsString(1, i) << " " << GetRowAsString(2, i) << "\n";
	}
}

std::string ConsoleGame::GetRowAsString(int z, int row)
{
	std::stringstream ss;
	// TODO: use actual value
	ss << "|" << " " << " " << " " << " " << " " << "|";
	return ss.str();
}
