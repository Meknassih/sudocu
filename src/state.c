#ifndef STATE_C
#define STATE_C

#include "definitions.h"
#include "model.c"
#include "backtrack.c"

void initCells(Cell cells[GRID_COLS][GRID_ROWS]) {
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            cells[i][j].isClue = 0;
            cells[i][j].value = -1;
        }
    }
}

void initGameState(GameState *gs) {
    initCells(gs->gridCells);
    do {
        genRandomCells(gs->gridCells, HARD);
        createNode(gs->gridCells, NULL, &(gs->root));
        debugPrint("Trying generation of intial puzzle");
    } while (!backtrack_solve(&(gs->root)));
    
    gs->cursorPos.x = 0;
    gs->cursorPos.y = 0;
    gs->state = ONGOING_PUZZLE;
    gs->shouldClose = false;
}

#endif
