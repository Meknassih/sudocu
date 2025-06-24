#ifndef MODEL_C
#define MODEL_C

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.c"
#include "solver.c"

int getClueCount(const Difficulty d) {
    switch (d) {
        case VERY_EASY:
            return 63;
        case EASY:
            return 53;
        case STANDARD:
            return 44;
        case HARD:
            return 35;
        case VERY_HARD:
        default:
            return 26;
    }
}

int genValidValue(const Cell cells[GRID_COLS][GRID_ROWS], const int x, const int y) {
    debugPrint("---genValidValue %d %d\n", x, y);
    Cell cellsClone[GRID_COLS][GRID_ROWS];
    cloneCells(GRID_COLS, GRID_ROWS, cells, cellsClone);

    for (int i = 1; i <= GRID_ROWS; i++) {
        cellsClone[x][y].value = i;
        Cell col[GRID_ROWS];
        Coord currentCoords = {x, y};
        Coord blockOrigin;
        getBlockOrigin(currentCoords, &blockOrigin);
        getCellsColumn(cells, col, x);
        const int isRowLegal = isLegalRow(cellsClone, y);
        const int isColLegal = isLegalCol(cellsClone, x);
        const int isBlockLegal = isLegalBlock(cellsClone, blockOrigin.x, blockOrigin.y);
        if (isRowLegal && isColLegal && isBlockLegal) {
            return i;
        }
        debugPrint("%d not legal at %d,%d (row? %d, col? %d, block? %d)\n", i, x, y, isRowLegal, isColLegal, isBlockLegal);
    }

    printGridHighlight(cells, x, y);
    fprintf(stderr, "Impossible to generate valid value\n");
    exit(EXIT_FAILURE);
}

void genRandomCells(Cell cells[GRID_COLS][GRID_ROWS], const Difficulty difficulty) {
    int clueCount = getClueCount(difficulty);
    for (int i = 0; i < clueCount; i++) {
        int cellSet = 0;

        while (!cellSet) {
            int r = rand() % CELL_COUNT;
            int x = r % GRID_ROWS,
                y = r / GRID_ROWS;
            if (cells[x][y].value == -1) {
                cells[x][y].value = genValidValue(cells, x, y);
                cells[x][y].isClue = 1;
                cellSet = 1;
            }
        }
    }
}

#endif /* ifndef MODEL_C */
