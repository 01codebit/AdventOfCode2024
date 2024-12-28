#include <stdio.h>
#include "data.h"
#include "..\common\printer.h"

typedef struct {
    int path_cells;
    int loop_options;
} result_struct;


result_struct evaluate_with_loops(map m, int debug);

int evaluate(map m, int max_iterations, int debug);

void reset_map(map m);

int brute_force_loops_search(map m, int max_iterations, int debug);