#include "Window.h"
#include "raylib.h"

Window::Window()
{
}

void Window::Open()
{
	const int screen_width = 800;
	const int screen_height = 450;

	InitWindow(screen_width, screen_height, "TicTacToe 3D");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("TicTacToe 3D!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();
}
