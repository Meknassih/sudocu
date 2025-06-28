#ifndef SOLVER_C
#define SOLVER_C


#include "definitions.h"
#include <stdio.h>
#include "util.c"

bool isLegalValue(const int v) {
    return (v == -1 || (v > 0 && v <= 9));
}

// Only works for square grids
bool isLegalAxis(const Cell cells[GRID_COLS]) {
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (!isLegalValue(cells[i].value || !isLegalValue(cells[j].value))) {
                fprintf(stderr, "Encountered illegal value");
                shouldClose = true;
            } else if (
                i == j ||
                cells[i].value == -1 ||
                cells[j].value == -1
            ) {
                continue;
            } else if (cells[j].value == cells[i].value) {
                debugPrint("\tIllegal axis because cell indexes %d (%d) == %d (%d)\n", i, cells[i].value, j, cells[j].value);
                return false;
            }
        }
    }

    return true;
}

bool isLegalRow(const Cell cells[GRID_COLS][GRID_ROWS], const int y) {
    debugPrint("\tTesting row %d:\n", y);
    printRow(cells[y]);
    const bool ret = isLegalAxis(cells[y]);
    return ret;
}

bool isLegalCol(const Cell cells[GRID_COLS][GRID_ROWS], const int x) {
    Cell col[GRID_ROWS];
    getCellsColumn(cells, col, x);
    debugPrint("\tTesting col %d:\n", x);
    printRow(col);
    return isLegalAxis(col);
}

bool isLegalBlock(const Cell cells[GRID_COLS][GRID_ROWS], const int x, int y) {
    Cell cellsAsAxis[GRID_BLOCK_SIZE*GRID_BLOCK_SIZE];
    flattenBlock(cells, x, y, cellsAsAxis);
    debugPrint("\tTesting flattened block at origin %d,%d :\n", x, y);
    printRow(cellsAsAxis);
    return isLegalAxis(cellsAsAxis);
}

bool isPuzzleSolved(const Cell cells[GRID_COLS][GRID_ROWS]) {
    for (int i = 0; i < GRID_ROWS; i++) {
        if (!isLegalRow(cells, i)) return false;
    }

    for (int i = 0; i < GRID_COLS; i++) {
        if (!isLegalCol(cells, i)) return false;
    }

    for (int i = 0; i < GRID_COLS / GRID_BLOCK_SIZE; i++) {
        for (int j = 0; j < GRID_COLS / GRID_BLOCK_SIZE; j++) {
            if (!isLegalBlock(cells, i * GRID_BLOCK_SIZE, j * GRID_BLOCK_SIZE))
                return false;
        }
    }

    return true;
}

#endif /* ifndef SOLVER_C */
