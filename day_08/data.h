#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

#define DEBUG 0

#define MAX_COUNT 1000
#define MAX_NUMBERS 200

typedef struct {
    char frequency;
    int x;
    int y;
} antenna;

typedef struct {
    char frequency;
    int x;
    int y;
} antinode;

typedef struct {
    char** data;
    int rows;
    int cols;
} map;

#endif 
