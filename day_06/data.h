#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

#define GUARD '^'
#define OBSTACLE  '#'

#define DEBUG 0

typedef struct {
    char** data;
    int rows;
    int cols;
    int guard_start_x;
    int guard_start_y;
} map;

#endif 
