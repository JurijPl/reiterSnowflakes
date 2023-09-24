#ifndef INCLUDE_GRID_H
#define INCLUDE_GRID_H

#include <stdlib.h>

// Neighbour offsets
int GRID_NEIGH_I_ODD_J[6]  = { -1, -1, -1,  0,  0,  1 };
int GRID_NEIGH_I_EVEN_J[6] = {  1,  1, -1,  0,  0,  1 };
int GRID_NEIGH_J[6]        = { -1,  1,  0, -1,  1,  0 };

#define neighs_i(col) (col % 2 == 0 ? GRID_NEIGH_I_EVEN_J : GRID_NEIGH_I_ODD_J)
#define neighs_j(col) GRID_NEIGH_J


float** gridCreate(int rows, int cols, float alpha, float beta)
{
    float** res = (float**) malloc(rows * sizeof(float*));
    float* vals = (float*) malloc(rows * cols * sizeof(float));

    for (int i = 0; i < rows; i++) {
        res[i] = &vals[i*rows];
        for (int j = 0; j < cols; j++) {
            res[i][j] = beta;
        }
    }

    res[rows/2][cols/2] = 1;
    return res;
}


void gridUpdate(float*** gridCurr, float*** gridNext)
{
    float** tmp = *gridCurr;
    *gridCurr = *gridNext;
    *gridNext = tmp;
}


void gridFree(float** grid)
{
    free(*grid);
    free(grid);
}

#endif
