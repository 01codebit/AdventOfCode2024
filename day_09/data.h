#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

typedef struct
{
    int *map;
    int length;
} disk_map;

typedef struct
{
    long long int *locations;
    int length;
} expansion;

#endif
