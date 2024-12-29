#pragma once
#ifndef _DATA
#define _DATA

#define CHUNK_SIZE 256

#define DEBUG 0

#define MAX_COUNT 1000
#define MAX_NUMBERS 200

typedef struct {
    long long total;
    long long* numbers;
    int count;
} calibration;

typedef struct {
    calibration* data;
    int count;
} calibrations;

#endif 
