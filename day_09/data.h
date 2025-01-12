#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

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
    ULLONG max_file_id;
} expansion;

#endif
