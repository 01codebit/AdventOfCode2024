#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

#define GUARD       '^'
#define OBSTACLE    '#'

#define MARKER_X    '+'
#define MARKER_V    '|'
#define MARKER_H    '-'

#define FREE        '.'

#define MAX_TURNS 200 
#define MAX_PATH 200

#define DEBUG 0

typedef struct {
    char** data;
    int rows;
    int cols;
    int guard_start_x;
    int guard_start_y;
} map;

#endif 
