#pragma once
#ifndef _DATA
#define _DATA

#define NUM_TYPE int

typedef struct
{
    char **matrix;
    int cols;
    int rows;
} data;

struct _node
{
    char name;
    NUM_TYPE area;
    NUM_TYPE perimeter;
    struct _node *next;
};

typedef struct _node region;

#endif