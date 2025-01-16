#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 30000

#define ULLONG long long

typedef struct
{
    int *map;
    int length;
} disk_map;

typedef struct
{
    ULLONG *locations;
    int length;
    int max_file_id;
    int *file_sizes;
} expansion;

#endif
