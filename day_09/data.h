#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 100000

#define LLONG long long

typedef struct
{
    int *map;
    int length;
} disk_map;

typedef struct
{
    LLONG *locations;
    int length;
    int max_file_id;
} expansion;

typedef struct 
{
    int file_id;
    int start;
    int count;
} n_index;

#endif
