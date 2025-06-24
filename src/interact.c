#ifndef INTERACT_C
#define INTERACT_C

#include "definitions.h"
#include "raylib.h"
#include <stdlib.h>
#include "util.c"

bool isShiftPressed() {
    return (
        IsKeyDown(KEY_LEFT_SHIFT) ||
        IsKeyDown(KEY_RIGHT_SHIFT)
    );
}

void handleCursorKeys(Coord *cursorCoords) {
    if (IsKeyPressed(KEY_H) || IsKeyPressedRepeat(KEY_H)) {
        if (isShiftPressed()) {
            cursorCoords->x = 0;
        } else {
            if ((cursorCoords->x - 1) >= 0)
                cursorCoords->x -= 1;
        }
    } else if (IsKeyPressed(KEY_J) || IsKeyPressedRepeat(KEY_J)) {
        if (isShiftPressed()) {
            cursorCoords->y = GRID_ROWS - 1;
        } else {
            if ((cursorCoords->y + 1) < GRID_ROWS)
                cursorCoords->y += 1;
        }
    } else if (IsKeyPressed(KEY_K) || IsKeyPressedRepeat(KEY_K)) {
        if (isShiftPressed()) {
            cursorCoords->y = 0;
        } else {
            if ((cursorCoords->y - 1) >= 0)
                cursorCoords->y -= 1;
        }
    } else if (IsKeyPressed(KEY_L) || IsKeyPressedRepeat(KEY_L)) {
        if (isShiftPressed()) {
            cursorCoords->x = GRID_COLS - 1;
        } else {
            if ((cursorCoords->x + 1) < GRID_COLS)
                cursorCoords->x += 1;
        }
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
        // TODO: use solver
    } else if (
        (IsKeyPressed(KEY_D) ||
        IsKeyPressed(KEY_BACKSPACE) ||
        IsKeyPressed(KEY_DELETE)) &&
        !selectedCell(gs)->isClue
    ) {
        selectedCell(gs)->value = -1;
    }
}

void handlePuzzleSolved(const Cell cells[GRID_COLS][GRID_ROWS]) {
    if (!isGridFull(cells)) return;
}

#endif
