#ifndef UTIL_C
#define UTIL_C

#include "definitions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void debugPrint(const char *format, ...) {
    if (!VERBOSE) return;

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void flatten2DCells(const int cols, const int rows, const Cell cells[cols][rows], Cell *flattened) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            const int id = (cols * i) + j;
            memcpy(&flattened[id], &(cells[i][j]), sizeof(Cell));
        }
    }
}

void flattenBlock(const Cell cells[GRID_COLS][GRID_ROWS], const int x, const int y, Cell *flattened) {
    for (int i = x; i < x + GRID_BLOCK_SIZE; i++) {
        for (int j = y; j < y + GRID_BLOCK_SIZE; j++) {
            const int id = (GRID_BLOCK_SIZE * (i - x)) + (j - y);
            memcpy(&flattened[id], &(cells[i][j]), sizeof(Cell));
        }
    }
}

void cloneCells(const int cols, const int rows, const Cell cells[cols][rows], Cell clone[cols][rows]) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            memcpy(&clone[i][j], &(cells[i][j]), sizeof(Cell));
        }
    }
}

void getCellsColumn(const Cell cells[GRID_COLS][GRID_ROWS], Cell col[GRID_ROWS], const int x) {
    for (int i = 0; i < GRID_ROWS; i++) {
        col[i].value = cells[i][x].value;
        col[i].isClue = cells[i][x].isClue;
    }
}

void getBlockOrigin(const Coord cell, Coord *origin) {
    origin->x = (cell.x / GRID_BLOCK_SIZE) * GRID_BLOCK_SIZE;
    origin->y = (cell.y / GRID_BLOCK_SIZE) * GRID_BLOCK_SIZE;
}

void printGrid(const Cell cells[GRID_COLS][GRID_ROWS]) {
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            debugPrint("\t%d\t", cells[i][j].value);
        }
        debugPrint("\n");
    }
}

void printGridHighlight(const Cell cells[GRID_COLS][GRID_ROWS], const int x, const int y) {
    for (int i = 0; i < GRID_COLS; i++) {
        debugPrint("%d\t", i);
    }
    debugPrint("\n");
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            if (j == 0) {
                debugPrint("%d |", i);
            }
            if (i == x && j == y)
                debugPrint("[%d]\t", cells[i][j].value);
            else
                debugPrint("%d\t", cells[i][j].value);
        }
        debugPrint("\n");
    }
}

void printRow(const Cell cells[GRID_COLS]) {
    for (int i = 0; i < GRID_COLS; i++) {
        debugPrint("%d\t", i);
    }
    debugPrint("\n");
    for (int i = 0; i < GRID_ROWS; i++) {
        debugPrint("%d\t", cells[i].value);
    }
    debugPrint("\n");
}

Cell* selectedCell(GameState *gs) {
    return &gs->gridCells[gs->cursorPos.x][gs->cursorPos.y];
}

void formatCoords(const int x, const int y, char *formatted) {
    char xStr[3], yStr[3];
    _itoa_s(x, xStr, sizeof xStr, 10);
    _itoa_s(y, yStr, sizeof yStr, 10);
    strcat_s(formatted, sizeof formatted, xStr);
    strcat_s(formatted, sizeof formatted, ", ");
    strcat_s(formatted, sizeof formatted, yStr);
}

bool findFirstEmptyCell(const Cell cells[GRID_COLS][GRID_ROWS], int *x, int *y) {
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (cells[i][j].value == -1) {
                if (x != NULL) *x = j;
                if (y != NULL) *y = i;
                return true;
            }
        }
    }
    
    return false;
}

bool isGridFull(const Cell cells[GRID_COLS][GRID_ROWS]) {
    return !findFirstEmptyCell(cells, NULL, NULL);
}

#endif /* ifndef UTIL_C */
