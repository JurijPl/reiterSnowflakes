#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#include "grid.h"


#define SAVE_IMGS_DURING_SIMULATION 0

#define OUTPUT_DIR "./out"
#define ITERS    1000
#define IMG_RATE 100

#define ROWS   500
#define COLS   500

#define ALPHA  2.003
#define BETA   0.40
#define GAMMA  0.0001

float** grid;
float** gridNext;
unsigned char** receptible;

unsigned char* image;


int isReceptible(int i, int j)
{
    // Frozen cell
    if (grid[i][j] >= 1)
        return 1;

    // Edge cell
    if (i == 0 || i == ROWS-1 || j == 0 || j == COLS-1)
        return 0;
    
    // Boundary cell
    for (int n = 0; n < 6; n++) {
        int ni = i + neighs_i(j)[n];
        int nj = j + neighs_j(j)[n];
        if (grid[ni][nj] >= 1)
            return 1;
    }

    return 0;
}


void iteration()
{
    // It is more efficient to compute receptibility beforehand
    for (int i = 1; i < ROWS-1; i++)
        for (int j = 1; j < COLS-1; j++)
            receptible[i][j] = isReceptible(i, j);

    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
            
            #if SAVE_IMGS_DURING_SIMULATION
            unsigned char px = grid[i][j] >= 1 ? 255 : 0;
            image[4 * (i * COLS + j) + 0] = px;
            image[4 * (i * COLS + j) + 1] = px;
            image[4 * (i * COLS + j) + 2] = px;
            image[4 * (i * COLS + j) + 3] = 255;
            #endif
            
            float s = grid[i][j];
            float u = receptible[i][j] ? 0 : s;

            float avg = 0;
            for (int n = 0; n < 6; n++) {
                int ni = i + neighs_i(j)[n];
                int nj = j + neighs_j(j)[n];
                if (!receptible[ni][nj])
                    avg += grid[ni][nj] / 6;
            }
            
            gridNext[i][j] = s + ALPHA / 2 * (avg - u) + GAMMA * receptible[i][j];
        }
    }

    gridUpdate(&grid, &gridNext);
}


int main(int argc, char* argv[])
{
    // Initialize grid
    grid = gridCreate(ROWS, COLS, ALPHA, BETA);
    gridNext = gridCreate(ROWS, COLS, ALPHA, BETA);
    image = malloc(ROWS * COLS * 4 * sizeof(unsigned char));

    // Allocate memory for storing receptibility
    unsigned char* tmp = calloc(ROWS * COLS, sizeof(unsigned char));
    receptible = malloc(ROWS * sizeof(unsigned char*));
    for (int i = 0; i < ROWS; i++)
        receptible[i] = &tmp[i * COLS];

    char fname[100];

    double dt = omp_get_wtime();

    for (int i = 0; i < ITERS; i++) {
        // Perform one iteration
        iteration();

        // Save image every k-th iteration
        #if SAVE_IMGS_DURING_SIMULATION
        if (i % IMG_RATE == 0) {
            sprintf(fname, "out/snezinka%d.png", i / IMG_RATE);
            stbi_write_png(fname, COLS, ROWS, 4, image, COLS * 4);
        }
        #endif
    }

    dt = omp_get_wtime() - dt;
    printf("%lf\n", dt);
    fflush(NULL);
}
