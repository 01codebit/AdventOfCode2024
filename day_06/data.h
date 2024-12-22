#pragma once
#define _DATA

#define CHUNK_SIZE 256

typedef struct {
    char** data;
    int rows;
    int cols;
} map;
