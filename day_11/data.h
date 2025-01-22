#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256
#define DEBUG 1

#define PART 1


struct _node
{
    char *value;
    struct _node *next;
};

typedef struct _node node;

typedef struct
{
    node *first;
    int count;
} linked_list;

#endif
