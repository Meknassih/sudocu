#ifndef DEFINTIONS_H
#define DEFINTIONS_H

#include <stdbool.h>
#define VERBOSE 0

#define GRID_COLS 9
#define GRID_ROWS 9
#define GRID_BLOCK_SIZE 3
#define GRID_FONT_SIZE 50
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define SKYBLUE_ALPHA (Color){ 102, 191, 255, 128 }
#define CELL_COUNT (GRID_COLS * GRID_ROWS)

#if (0 != GRID_ROWS % GRID_BLOCK_SIZE || 0 != GRID_COLS % GRID_BLOCK_SIZE)
#error "Grid size and group size not supported"
#endif

bool shouldClose = false;

typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    int value;
    int isClue;
} Cell;

typedef enum {
    TITLE_SCREEN,
    ONGOING_PUZZLE,
    SOLVED_PUZZLE,
} State;

typedef struct {
    Cell gridCells[GRID_COLS][GRID_ROWS];
    Coord cursorPos;
    State state;
    bool shouldClose;
} GameState;

typedef enum {
    VERY_EASY = 0,
    EASY = 1,
    STANDARD = 2,
    HARD = 3,
    VERY_HARD = 4,
} Difficulty;


#endif
