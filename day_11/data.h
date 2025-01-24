#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256
#define DEBUG 1

#define MAX_NODES 200000
#define MAX_LISTS 10000

struct _node
{
    long long value;
    struct _node *next;
};

typedef struct _node node;

typedef struct
{
    node *first;
    // node *pre_last;
    long long count;
} linked_list;

#endif
