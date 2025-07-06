#ifndef INTERACT_C
#define INTERACT_C

#include "definitions.h"
#include "raylib.h"
#include <stdlib.h>
#include "util.c"
#include "solver.c"
#include "backtrack.c"

bool isShiftPressed() {
    return (
        IsKeyDown(KEY_LEFT_SHIFT) ||
        IsKeyDown(KEY_RIGHT_SHIFT)
    );
}

void handleCursorKeys(GameState *gs) {
    if (IsKeyPressed(KEY_H) || IsKeyPressedRepeat(KEY_H)) {
        if (isShiftPressed()) {
            gs->cursorPos.x = 0;
        } else {
            if ((gs->cursorPos.x - 1) >= 0)
                gs->cursorPos.x -= 1;
        }

        if (gs->state == SOLVED_PUZZLE) gs->state = ONGOING_PUZZLE;
    } else if (IsKeyPressed(KEY_J) || IsKeyPressedRepeat(KEY_J)) {
        if (isShiftPressed()) {
            gs->cursorPos.y = GRID_ROWS - 1;
        } else {
            if ((gs->cursorPos.y + 1) < GRID_ROWS)
                gs->cursorPos.y += 1;
        }

        if (gs->state == SOLVED_PUZZLE) gs->state = ONGOING_PUZZLE;
    } else if (IsKeyPressed(KEY_K) || IsKeyPressedRepeat(KEY_K)) {
        if (isShiftPressed()) {
            gs->cursorPos.y = 0;
        } else {
            if ((gs->cursorPos.y - 1) >= 0)
                gs->cursorPos.y -= 1;
        }

        if (gs->state == SOLVED_PUZZLE) gs->state = ONGOING_PUZZLE;
    } else if (IsKeyPressed(KEY_L) || IsKeyPressedRepeat(KEY_L)) {
        if (isShiftPressed()) {
            gs->cursorPos.x = GRID_COLS - 1;
        } else {
            if ((gs->cursorPos.x + 1) < GRID_COLS)
                gs->cursorPos.x += 1;
        }

        if (gs->state == SOLVED_PUZZLE) gs->state = ONGOING_PUZZLE;
    }
}

void handleValueInput(const Coord cursorCoords, Cell cells[GRID_COLS][GRID_ROWS]) {
    if (cells[cursorCoords.x][cursorCoords.y].isClue) return;

    // KEY_* enums in Raylib are based on ASCII values
    const int key = GetKeyPressed();
    if (key >= 49 && key <= 57) {
        char c[2] = {(char)key, '\0'};
        const int i = atoi(c);
        cells[cursorCoords.x][cursorCoords.y].value = i;
    }
}

void handleCommandKeys(GameState *gs) {
    if (IsKeyPressed(KEY_C)) {
        if (isPuzzleSolved(gs->gridCells)) {
            gs->state = SOLVED_PUZZLE;
        } else {
            gs->state = FAILED_SOLVING;
        }
    } else if (
        (IsKeyPressed(KEY_D) ||
        IsKeyPressed(KEY_BACKSPACE) ||
        IsKeyPressed(KEY_DELETE)) &&
        !selectedCell(gs)->isClue
    ) {
        selectedCell(gs)->value = -1;
    } else if (IsKeyPressed(KEY_S)) {
        if (backtrack_solve(&(gs->root))) {
            printf("Found solution\n");
            printGrid(gs->root.cells);
            gs->state = SOLVED_PUZZLE;
        } else {
            printf("No solution found\n");
            gs->state = FAILED_SOLVING;
        }
    }
}

void handleFailSolvDialogCommands(State *s) {
    if (IsKeyPressed(KEY_ENTER)) {
        *s = ONGOING_PUZZLE;
    }
}

#endif
