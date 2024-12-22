#pragma once
#define _DATA

#define CHUNK_SIZE 8

typedef struct {
    int prev;
    int next;
} rule;

typedef struct {
    int* chapters;
    int size;
} update;

typedef struct {
    rule* rules;
    int rules_count;
    update* updates; 
    int updates_count;
} data;
