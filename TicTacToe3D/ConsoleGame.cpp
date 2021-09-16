#include "ConsoleGame.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdexcept>

ConsoleGame::ConsoleGame() : game_()
{
	MainLoop();
}

void ConsoleGame::MainLoop()
{
	auto turn_over = false;
	while (!game_ended_) {
		DrawScore();
		DrawBoard();
		auto current_player = game_.GetCurrentPlayer().GetSymbol();
		std::cout << "Turn of " << current_player << "\n";
		std::cout << "Type \"Quit\" to quit: ";
		std::string command;
		std::cin >> command;
		std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {return std::tolower(c); });

		if (command == "quit" || command == "q") {
			game_ended_ = true;
		}
		else if (command.length() == 2) {
			char column = command[0];
			int row = stoi(command.substr(1));
			Vec3 vec = GetPosVec(column, row);
			if (!game_.SetCell(vec.x, vec.y, vec.z, current_player)) {
				std::cout << "Invalid move!\n";
			}
			else {
				turn_over = true;
			}
		}
		else {
			std::cout << "Unrecognized command!\n";
		}

		if (turn_over) {
			turn_over = false;
			game_.EndTurn();
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

void ConsoleGame::DrawScore()
{
	std::cout << "SCORE: ";

	bool comma = false;

	for (auto& player : game_.GetPlayers()) {
		if (comma) {
			std::cout << ", ";
		}
		comma = true;
		std::cout << player.GetSymbol() << ": " << player.GetPoints();
	}

	std::cout << "\n";
}

std::string ConsoleGame::GetRowAsString(int z, int row)
{
	std::stringstream ss;
	// TODO: use actual value
	ss << "|" << GetCellCharAt(0, row, z) << " " << GetCellCharAt(1, row, z) << " " << GetCellCharAt(2, row, z) << "|";
	return ss.str();
}

char ConsoleGame::GetCellCharAt(int x, int y, int z)
{
	return game_.GetCell(x, y, z).GetContent();
}

Vec3 ConsoleGame::GetPosVec(char column, int row)
{
	int x, y, z;
	y = row - 1;
	switch (column)
	{
	case 'a':
		z = 0;
		x = 0;
		break;
	case 'b':
		z = 0;
		x = 1;
		break;
	case 'c':
		z = 0;
		x = 2;
		break;
	case 'd':
		z = 1;
		x = 0;
		break;
	case 'e':
		z = 1;
		x = 1;
		break;
	case 'f':
		z = 1;
		x = 2;
		break;
	case 'g':
		z = 2;
		x = 0;
		break;
	case 'h':
		z = 2;
		x = 1;
		break;
	case 'i':
		z = 2;
		x = 2;
		break;
	default:
		throw std::invalid_argument("Invalid column");
		break;
	}
	return Vec3{ x, y, z };
}
