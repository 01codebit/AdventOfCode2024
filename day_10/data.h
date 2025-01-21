#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256
#define DEBUG 0

#define PART 2

typedef struct
{
    const int **data;
    int rows;
    int cols;
} map;

#endif
