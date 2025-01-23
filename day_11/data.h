#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256
#define DEBUG 1

#define PART 1


struct _node
{
    long long value;
    struct _node *next;
};

typedef struct _node node;

typedef struct
{
    node *first;
    long long count;
} linked_list;

#endif
