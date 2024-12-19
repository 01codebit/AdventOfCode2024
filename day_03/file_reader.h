#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LEN 256
#define CHUNK_SIZE 100

typedef struct {
    char* text;
    int length;
} data;

data read_file(char* filename);

char* read_text(char* filename);