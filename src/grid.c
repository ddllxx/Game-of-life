#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

// alloc memory to save the data of the grid
int **alloc_grid(const Grid *pGrid);

// get data from a file
void init_grid(Grid *pGrid, FILE *fp) {
    if (pGrid == NULL || fp == NULL) {
        ERREXIR("error NULL pointer");
    }
    fscanf(fp, "%d%d", &pGrid->row, &pGrid->col);
    pGrid->gridData = alloc_grid(pGrid);
    for (int i = 0; i < pGrid->row; i++) {
        for (int j = 0; j < pGrid->col; j++) {
            fscanf(fp, "%d", &pGrid->gridData[i][j]);
        }
    }
}

void init_grid2(Grid *pGrid, int row, int col, int value) {
    if (pGrid == NULL) {
        ERREXIR("error NULL pointer");
    }
    pGrid->row = row;
    pGrid->col = col;
    pGrid->gridData = alloc_grid(pGrid);
    for (int i = 0; i < pGrid->row; i++) {
        for (int j = 0; j < pGrid->col; j++) {
            pGrid->gridData[i][j] = value;
        }
    }
}

void switch_grid_cell(Grid *pGrid, int index_i, int index_j) {
    if (pGrid == NULL) {
        ERREXIR("error NULL pointer");
    }
    if (index_i < 0 || index_j < 0 || index_i >= pGrid->row || index_j >= pGrid->col) {
        ERREXIR("index out of range.");
    }
    pGrid->gridData[index_i][index_j] = !pGrid->gridData[index_i][index_j];
}

// calc the number of neighbor of a cell which pos is (i, j)
int calc_neighbor(const Grid *pGrid, int i, int j) {
    if (pGrid == NULL) {
        ERREXIR("error NULL pointer");
    }
    int sum = 0;
    // get the border limit
    int start_i = i > 0 ? i - 1 : 0;
    int start_j = j > 0 ? j - 1 : 0;
    int end_i = i < pGrid->row - 1 ? i + 1 : i;
    int end_j = j < pGrid->col - 1 ? j + 1 : j;
    for (int p = start_i; p <= end_i; p++) {
        for (int q = start_j; q <= end_j; q++) {
            sum += pGrid->gridData[p][q];
        }
    }
    sum -= pGrid->gridData[i][j];
    return sum;
}

// update the data to get the next status

// update the data to get the next status
int update_grid(Grid *pGrid) {
    if (pGrid == NULL) {
        ERREXIR("error NULL pointer");
    }
    // flag
    int change = 0;
    // realloc a new data memory
    int **new_data = alloc_grid(pGrid);
    for (int i = 0; i < pGrid->row; i++) {
        for (int j = 0; j < pGrid->col; j++) {
            int count_neighbor = calc_neighbor(pGrid, i, j);
            // die
            if (count_neighbor <= 1) {
                new_data[i][j] = 0;
            } else if (count_neighbor == 2) {       // just right
                new_data[i][j] = pGrid->gridData[i][j];
            } else if (count_neighbor == 3) {       // resurrect
                new_data[i][j] = 1;
            } else {                                // die
                new_data[i][j] = 0;
            }
            if (new_data[i][j] != pGrid->gridData[i][j]) {
                change = 1;
            }
        }
    }
    // destroy old data
    free_grid(pGrid);
    // update
    pGrid->gridData = new_data;
    return change;
}

// alloc memory to save the data of the grid
int **alloc_grid(const Grid *pGrid) {
    int **data = malloc(pGrid->row * sizeof(int *));
    for (int i = 0; i < pGrid->row; ++i) {
        data[i] = malloc(pGrid->col * sizeof(int));
    }
    return data;
}

// destroy the data memory
void free_grid(Grid *pGrid) {
    for (int i = 0; i < pGrid->row; i++) {
        free(pGrid->gridData[i]);
    }
    free(pGrid->gridData);
}

// dave the data to a file
void output_file(const Grid *pGrid, int delay, const char *filename) {
    if (pGrid == NULL) {
        ERREXIR("error NULL pointer");
    }
    FILE * output = fopen(filename, "w+");
    if (output == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
    }
    fprintf(output, "%d %d %d %d\n", 0, delay, pGrid->row, pGrid->col);
    for (int i = 0; i < pGrid->row; i++) {
        for (int j = 0; j < pGrid->col; j++) {
            fprintf(output, "%d", pGrid->gridData[i][j]);
            if (j != pGrid->col - 1) {
                fputc(' ', output);
            }
        }
        if (i != pGrid->row - 1) {
            fputs("\n", output);
        }
    }
    fclose(output);
}

