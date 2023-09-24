# Reiter model for snowflake growth in C

A school project for which we visualised the growth of snowflakes using Reiter's model (explained [here](https://itp.uni-frankfurt.de/~gros/StudentProjects/Projects_2020/projekt_yan_huck/).

## Content

- **stb_image_write.h**: header file which contains methods for the visualization of snowflake generation ([link](https://github.com/nothings/stb/tree/master)).
- **grid.h**: header file, which contains methods for the initialization of the grid in which snowflakes are generated.
- **reiter.c**: base C file, containing the algorithm for snowflake generation
- **reiter_pthreads.c**: C file, using pThreads multithreading used to measure speed-ups in snowflake generation.
- **reiter_omp.c**: C file, using OpenMP multi-processing ...
