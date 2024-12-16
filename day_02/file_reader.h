#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LEN 256
#define CHUNK_SIZE 100

typedef struct {
    int** array;
    int reports_count;
    int* levels_counts;
} data;

data read_file(char* filename);