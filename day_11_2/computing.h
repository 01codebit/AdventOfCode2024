#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "file_reader.h"

#include "data.h"

char *get_time_string(float time);

long long int convert_to_values_array(long long int *values, char *str, long long chunks, int debug);

long long int compute_depth_r(long long int *values, int values_count, int target);