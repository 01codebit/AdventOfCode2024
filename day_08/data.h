#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 12048

#define DEBUG 0

#define MAX_COUNT 1000
#define MAX_NUMBERS 200

typedef struct {
    int id;
    char frequency;
    int x;
    int y;
} antenna;

typedef struct {
    antenna* list;
    int count;
} antennas;

typedef struct {
    char frequency;
    int x;
    int y;
} antinode;

typedef struct {
    antinode* list;
    int count;
} antinodes;


typedef struct {
    char** data;
    int rows;
    int cols;
} map;

#endif 
