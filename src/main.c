/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "definitions.h"
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "board.c"
#include "state.c"
#include "interact.c"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sudocu");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	// Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	GameState gs;
	initGameState(&gs);
	
	// game loop
	while (!WindowShouldClose() && !shouldClose)
	{
		switch (gs.state) {
			case TITLE_SCREEN:
				break;
			case ONGOING_PUZZLE:
				// Input checks
				handleCommandKeys(&gs);
				handleCursorKeys(&gs);
				handleValueInput(gs.cursorPos, gs.gridCells);

				// Drawing
				BeginDrawing();
				// Setup the back buffer for drawing (clear color and depth buffers)
				ClearBackground(MAROON);

				drawBoard();
				drawValues(gs.gridCells);
				drawCursor(gs.cursorPos);
				drawStatusBar(gs);

				// end the frame and get ready for the next one  (display frame, poll input, etc...)
				EndDrawing();
				break;
			case SOLVED_PUZZLE:
				handleCommandKeys(&gs);
				handleCursorKeys(&gs);
				handleValueInput(gs.cursorPos, gs.gridCells);

				BeginDrawing();
				ClearBackground(DARKGREEN);

				drawBoard();
				drawValues(gs.gridCells);
				drawCursor(gs.cursorPos);
				drawStatusBar(gs);

				EndDrawing();
				break;
			case FAILED_SOLVING:
				handleFailSolvDialogCommands(&gs.state);

				BeginDrawing();
				ClearBackground(MAROON);

				drawBoard();
				drawValues(gs.gridCells);
				drawFailedSolvingDialog();

				EndDrawing();
				break;
			default:
				fprintf(stderr, "Unknown state gs->state\n");
				shouldClose = true;
		}
	}

	// cleanup
	// unload our texture so it can be cleaned up
	// UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
