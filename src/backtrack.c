#ifndef BACKTRACK_C
#define BACKTRACK_C

#include "definitions.h"
#include "solver.c"

void createNode(Cell cells[GRID_COLS][GRID_ROWS], BTNode *parent, BTNode *n) {
    if (n == NULL) {
        perror("Attempting to create node with NULL pointer");
        return;
    }

    cloneCells(GRID_COLS, GRID_ROWS, cells, n->cells);
    n->parent = parent;
    n->children = NULL;
    n->childrenCount = 0;
    n->childrenTried = NULL;
    n->triedCount = 0;
}

void destroyAllChildren(BTNode *n) {
}

void setNodeUniqueChild(BTNode *node, BTNode *child) {
}

void setNodeChildren(BTNode *n) {
    int x = 0, y = 0;
    bool foundEmptyCell = findFirstEmptyCell(n->cells, &x, &y);
    if (!foundEmptyCell) {
        n->childrenCount = 0;
        n->triedCount = 0;
        if (n->children != NULL) {
            free(n->children);
            n->children = NULL;
        }
        if (n->childrenTried != NULL) {
            // Not freeing its children becase they are just refs to n->children
            free(n->childrenTried);
            n->childrenTried = NULL;
        }
        return;
    } else if (n->childrenCount != 0) {
        // We used to free both pointers here but ended up
        // removing it to avoid free() into malloc() instantly
        n->childrenCount = 0;
        n->triedCount = 0;
    } else {
        n->children = malloc(sizeof(BTNode) * GRID_COLS);
        n->childrenTried = malloc(sizeof(BTNode*) * GRID_COLS);
    }

    for (int i = 0; i < GRID_ROWS; i++) {
        Cell clone[GRID_COLS][GRID_ROWS];
        cloneCells(GRID_COLS, GRID_ROWS, n->cells, clone);
        clone[y][x].value = i + 1;
        createNode(clone, n, &(n->children[i]));
        n->childrenCount += 1;
    }
}

BTNode *nextUntriedChild(BTNode *n) {
    for (int i = 0; i < n->childrenCount; i++) {
        bool found = false;
        for (int j = 0; j < n->triedCount; j++) {
            if (&(n->children[i]) == n->childrenTried[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            n->childrenTried[n->triedCount] = &(n->children[i]);
            n->triedCount += 1;
            return &(n->children[i]);
        }
    }

    perror("All children of %p have been tried without success");
    return NULL;
}

bool reject(const BTNode c) {
    return !isGridLegal(c.cells);
}

bool accept(const BTNode c) {
    return isPuzzleSolved(c.cells);
}

BTNode *next(BTNode *c) {
    if (c->childrenCount == 0) {
        setNodeChildren(c);
    }

    return nextUntriedChild(c);
}

bool backtrack_solve(BTNode *n) {
    if (reject(*n)) return false;
    else if (accept(*n)) return true;
    
    bool solved = false;
    BTNode *n2 = next(n);
    while (n2 != NULL && !solved) {
        solved = backtrack_solve(n2);
        n2 = next(n2);
    }

    return solved;
}

// TODO: implement freeing the whole graph from one of its nodes

#endif /* ifndef BACKTRACK_C */
