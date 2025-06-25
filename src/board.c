#ifndef BOARD_C
#define BOARD_C

#include "definitions.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "util.c"

#define gapX ((WINDOW_WIDTH / GRID_COLS) / GRID_COLS)
#define gapY ((WINDOW_HEIGHT / GRID_ROWS) / GRID_ROWS)

#define gapBorderingX (gapX * 2)
#define gapBorderingY (gapY * 2)

#define totalGapsX ((gapBorderingX * 2) + (gapX * (GRID_COLS - 1)))
#define totalGapsY ((gapBorderingY * 2) + (gapY * (GRID_ROWS - 1)))

#define cellWidth ((WINDOW_WIDTH - totalGapsX) / GRID_COLS)
#define cellHeight ((WINDOW_HEIGHT - totalGapsY) / GRID_ROWS)

void drawBoard() {
    // Draw cells
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            int x = i * (cellWidth + gapX) + gapBorderingX,
                y = j * (cellHeight + gapX) + gapBorderingY;

            DrawRectangle(x, y, cellWidth, cellHeight, WHITE);
        }
    }

    // Draw separators
    int hGroups = GRID_COLS / GRID_BLOCK_SIZE,
        vGroups = GRID_ROWS / GRID_BLOCK_SIZE;

    int groupWidth = GRID_BLOCK_SIZE * (cellWidth + gapX), 
        groupHeight = GRID_BLOCK_SIZE * (cellHeight + gapY); 

    for (int i = 1; i < hGroups; i++) {
        int x = i * groupWidth + gapX,
            y = gapBorderingY,
            w = gapX,
            h = WINDOW_HEIGHT - 2 * gapBorderingY;

        DrawRectangle(x, y, w, h, BLACK);
    }

    for (int i = 1; i < vGroups; i++) {
        int x = gapBorderingX,
            y = i * groupHeight + gapY,
            w = WINDOW_WIDTH - 2 * gapBorderingX,
            h = gapY;

        DrawRectangle(x, y, w, h, BLACK);
    }
}   

void drawCursor(const Coord coords) {
    int x = gapBorderingX + coords.x * (cellWidth + gapX),
        y = gapBorderingY + coords.y * (cellWidth + gapY);

    DrawRectangle(x, y, cellWidth, cellHeight, SKYBLUE_ALPHA);
}

void drawValues(const Cell cells[GRID_COLS][GRID_ROWS]) {
    // Draw digits
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            Color color;
            if (cells[i][j].value == -1) continue;

            if (cells[i][j].isClue) color = GRAY;
            else color = BLACK;

            int x = i * (cellWidth + gapX) + gapBorderingX + (cellWidth / 4),
                y = j * (cellHeight + gapX) + gapBorderingY + (cellHeight / 4);

            char str[8]; 
            _itoa_s(cells[i][j].value, str, 8, 10);
            DrawText(str, x, y, GRID_FONT_SIZE, color);
        }
    }
}

void drawStatusBar(GameState gs) {
    int y = WINDOW_HEIGHT - gapBorderingY,
        x = gapBorderingX,
        strcat_err = 0;

    char statusText[2048] = "",
        err_str[1024] = "",
        cStr[21] = "c: check solution  ",
        hjklStr[14] = "hjkl: move  ",
        shiftHjklStr[20] = "Shift+hjkl: jump  ",
        dStr[12] = "d: delete  ";

    if (isGridFull(gs.gridCells)) {
        strcat_err = strcat_s(statusText, sizeof statusText, cStr);
        if (strcat_err != 0) {
            strerror_s(err_str, sizeof err_str, strcat_err);
            fprintf(stderr, "%s\n", err_str);
        }
    }
    
    if (!selectedCell(&gs)->isClue && selectedCell(&gs)->value != -1) {
        strcat_err = strcat_s(statusText, sizeof statusText, dStr);
        if (strcat_err != 0) {
            strerror_s(err_str, sizeof err_str, strcat_err);
            fprintf(stderr, "%s\n", err_str);
        }
    }

    strcat_err = strcat_s(statusText, sizeof statusText, hjklStr);
    if (strcat_err != 0) {
        strerror_s(err_str, sizeof err_str, strcat_err);
        fprintf(stderr, "%s\n", err_str);
    }
    strcat_err = strcat_s(statusText, sizeof statusText, shiftHjklStr);
    if (strcat_err != 0) {
        strerror_s(err_str, sizeof err_str, strcat_err);
        fprintf(stderr, "%s\n", err_str);
    }

    DrawText(statusText, x, y, 20, GREEN);
}

void drawFailedSolvingDialog() {
    DrawRectangle(0, WINDOW_HEIGHT/3, WINDOW_WIDTH, WINDOW_HEIGHT/3, DARKBROWN);
    const char mainText[40] = "There is an illegal cell in the puzzle.",
        helpText[13] = "enter: close";
    const int mainTextWidth = MeasureText(mainText, 40),
        helpTextWidth = MeasureText(helpText, 30);
    DrawText(mainText,
             (WINDOW_WIDTH/2) - (mainTextWidth/2),
             (WINDOW_HEIGHT/3) + 80,
             40,
             WHITE);
    DrawText(helpText,
             (WINDOW_WIDTH/2) - (helpTextWidth/2),
             (WINDOW_HEIGHT/3) * 2 - 80,
             30,
             WHITE);
}

#endif
