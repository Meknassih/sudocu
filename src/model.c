#ifndef MODEL_C
#define MODEL_C

#include "definitions.h"
#include <debugapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.c"
#include "solver.c"

int getClueCount(const Difficulty d) {
    switch (d) {
        case FULL_DEBUG:
            return 81;
        case ONE_DEBUG:
            return 80;
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
        debugPrint("---\n");
        const int isRowLegal = isLegalRow(cellsClone, x); // x,y inversed problem !
        debugPrint("---\n");
        const int isColLegal = isLegalCol(cellsClone, y); // x,y inversed problem !
        debugPrint("---\n");
        const int isBlockLegal = isLegalBlock(cellsClone, blockOrigin.x, blockOrigin.y);
        debugPrint("---\n");
        if (isRowLegal && isColLegal && isBlockLegal) {
            return i;
        }
        debugPrint("%d not legal at %d,%d (row? %d, col? %d, block? %d)\n", i, x, y, isRowLegal, isColLegal, isBlockLegal);
    }

    printGridHighlight(cells, x, y);
    fprintf(stderr, "Impossible to generate valid value\n");
    shouldClose = true;
    DebugBreak();
    return -1;
}

bool getRandomFilledCell(Cell cells[GRID_COLS][GRID_ROWS], int *x, int *y, bool excludeClues) {
    for (int i = 0; i < (GRID_ROWS * GRID_ROWS); i++) {
        int r = rand() % CELL_COUNT;
        int cx = r % GRID_ROWS,
            cy = r / GRID_ROWS;
        if (
            cells[cy][cx].value != -1 &&
            ((excludeClues && !cells[cy][cx].isClue) || !excludeClues)
        ) {
            *x = cx;
            *y = cy;
            return true;
        }
    }
    
    return false;
}

void genInitialCells(Cell cells[GRID_COLS][GRID_ROWS]) {
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            cells[i][j].value = ((j % 3) * 3) + ((i % 3) + 1);
            cells[i][j].isClue = true;
        }
    }
}

void genInitialCellsStatic(Cell cells[GRID_COLS][GRID_ROWS]) {
    Cell initial[GRID_COLS][GRID_ROWS] = {
        {{1, true}, {4, true}, {7, true}, {2, true}, {5, true}, {8, true}, {3, true}, {6, true}, {9, true}},
        {{2, true}, {5, true}, {8, true}, {3, true}, {6, true}, {9, true}, {1, true}, {4, true}, {7, true}},
        {{3, true}, {6, true}, {9, true}, {1, true}, {4, true}, {7, true}, {2, true}, {5, true}, {8, true}},
        {{4, true}, {7, true}, {1, true}, {5, true}, {8, true}, {2, true}, {6, true}, {9, true}, {3, true}},
        {{5, true}, {8, true}, {2, true}, {6, true}, {9, true}, {3, true}, {4, true}, {7, true}, {1, true}},
        {{6, true}, {9, true}, {3, true}, {4, true}, {7, true}, {1, true}, {5, true}, {8, true}, {2, true}},
        {{7, true}, {1, true}, {4, true}, {8, true}, {2, true}, {5, true}, {9, true}, {3, true}, {6, true}},
        {{8, true}, {2, true}, {5, true}, {9, true}, {3, true}, {6, true}, {7, true}, {1, true}, {4, true}},
        {{9, true}, {3, true}, {6, true}, {7, true}, {1, true}, {4, true}, {8, true}, {2, true}, {5, true}},
    };

    cloneCells(GRID_COLS, GRID_ROWS, initial, cells);
}

void swapBlockCol(
    Cell cells[GRID_COLS][GRID_ROWS],
    const Coord sourceCoords,
    const Coord destCoords
) {
    Cell tempCells[3];
    for (int i = 0; i < 3; i++) {
        tempCells[i] = cells[destCoords.x][destCoords.y + i];
    }
    for (int i = 0; i < 3; i++) {
        cells[destCoords.x][destCoords.y + i] = cells[sourceCoords.x][sourceCoords.y + i];
    }
    for (int i = 0; i < 3; i++) {
        cells[sourceCoords.x][sourceCoords.y + i] = tempCells[i];
    }
}

void cycleBlockCols(Cell cells[GRID_COLS][GRID_ROWS], Coord blockOrigin) {
    Coord s = {blockOrigin.x + 2, blockOrigin.y},
        d = {blockOrigin.x + 1, blockOrigin.y};
    swapBlockCol(cells, s, d);
    s.x = blockOrigin.x + 1; s.y = blockOrigin.y;
    d.x = blockOrigin.x; d.y = blockOrigin.y;
    swapBlockCol(cells, s, d);
}

void swapBlockRow(
    Cell cells[GRID_COLS][GRID_ROWS],
    const Coord sourceCoords,
    const Coord destCoords
) {
    Cell tempCells[3];
    for (int i = 0; i < 3; i++) {
        tempCells[i] = cells[destCoords.x + i][destCoords.y];
    }
    for (int i = 0; i < 3; i++) {
        cells[destCoords.x + i][destCoords.y] = cells[sourceCoords.x + i][sourceCoords.y];
    }
    for (int i = 0; i < 3; i++) {
        cells[sourceCoords.x + i][sourceCoords.y] = tempCells[i];
    }
}

void cycleBlockRows(Cell cells[GRID_COLS][GRID_ROWS], Coord blockOrigin) {
    Coord s = {blockOrigin.x, blockOrigin.y + 2},
        d = {blockOrigin.x, blockOrigin.y + 1};
    swapBlockRow(cells, s, d);
    s.x = blockOrigin.x; s.y = blockOrigin.y + 1;
    d.x = blockOrigin.x; d.y = blockOrigin.y;
    swapBlockRow(cells, s, d);
}

void swapNumber(Cell cells[GRID_COLS][GRID_ROWS], const int a, const int b) {
    debugPrint("Swapping %d with %d\n", a, b);
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (cells[i][j].value == a) cells[i][j].value = b;
            else if (cells[i][j].value == b) cells[i][j].value = a;
        }
    }
}

void swapNumbers(Cell cells[GRID_COLS][GRID_ROWS]) {
    const int countToSwap = 6; // out of 9
    int numbersToSwapSrc[6] = {0},
        numbersToSwapDest[6] = {0};
    mapNumbersToRand(numbersToSwapSrc, numbersToSwapDest, countToSwap);
    for (int i = 0; i < countToSwap; i++) {
        swapNumber(cells, numbersToSwapSrc[i], numbersToSwapDest[i]);
    }
}

void genRandomCells(Cell cells[GRID_COLS][GRID_ROWS], const Difficulty difficulty) {
    int clueCount = getClueCount(difficulty);
    genInitialCellsStatic(cells);
    swapNumbers(cells);

    // shuffleCells(cells); // TODO
    for (int i = 0; i < (81-clueCount); i++) {
        int x, y;
        if (getRandomFilledCell(cells, &x, &y, false)) {
            cells[y][x].isClue = false;
            cells[y][x].value = -1;
        } else {
            perror("Could not remove cell to reach clue count");
            break;
        }
    }
}

void genRandomCells_old(Cell cells[GRID_COLS][GRID_ROWS], const Difficulty difficulty) {
    // TODO: start with a known valid grid, shuffle it, remove a random cell
    // and check if it is still solvable until the difficulty number is reached
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
