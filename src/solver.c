#ifndef SOLVER_C
#define SOLVER_C


#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.c"

int isLegalValue(const int v) {
    return (v == -1 || (v > 0 && v <= 9));
}

// Only works for square grids
int isLegalAxis(const Cell cells[GRID_COLS]) {
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (!isLegalValue(cells[i].value || !isLegalValue(cells[j].value))) {
                fprintf(stderr, "Encountered illegal value");
                exit(EXIT_FAILURE);
            } else if (
                i == j ||
                cells[i].value == -1 ||
                cells[j].value == -1
            ) {
                continue;
            } else if (cells[j].value == cells[i].value) {
                debugPrint("Illegal axis because cell indexes %d (%d) == %d (%d)\n", i, cells[i].value, j, cells[j].value);
                return 0;
            }
        }
    }

    return 1;
}

int isLegalBlock(const Cell cells[GRID_COLS][GRID_ROWS], const int x, int y) {
    Cell cellsAsAxis[GRID_BLOCK_SIZE*GRID_BLOCK_SIZE];
    flattenBlock(cells, x, y, cellsAsAxis);
    debugPrint("Flattened block at origin %d,%d :\n", y, x);
    printRow(cellsAsAxis);
    return isLegalAxis(cellsAsAxis);
}


#endif /* ifndef SOLVER_C */
