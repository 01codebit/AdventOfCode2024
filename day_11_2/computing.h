#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "file_reader.h"

#include "data.h"

void cache_pool_init();

void free_cache_pool();

LLINT convert_to_values_array(LLINT *values, char *str, LLINT chunks, int debug);

LLINT compute_depth_r(LLINT *values, int values_count, int target, int use_cache);