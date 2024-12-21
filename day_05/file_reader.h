#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 8

typedef struct {
    int prev;
    int next;
} rule;

typedef struct {
    int* chapters;
} update;

typedef struct {
    rule* rules;
    update* updates; 
} data;

data read_data(char* filename);