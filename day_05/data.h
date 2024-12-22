#pragma once
#define _DATA

#define CHUNK_SIZE 256

typedef struct {
    int prev;
    int next;
} rule;

typedef struct {
    int* pages;
    int size;
} update;

typedef struct {
    rule* rules;
    int rules_count;
    update* updates; 
    int updates_count;
} data;
