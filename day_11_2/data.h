#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 200000
#define DEBUG 1

#define OUTPUT 0

#define MAX_NODES 200000

typedef struct
{
    int step;
    long long value;
} node;

typedef struct
{
    long long input_list_id;
    long long output_list_id_1;
    long long output_list_id_2;
    int steps;
    int debug; 
    long long nodes_count;
} thread_args;


#endif
