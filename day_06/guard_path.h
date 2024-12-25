#include <stdio.h>
#include "data.h"
#include "..\common\printer.h"

typedef struct {
    int path_cells;
    int loop_options;
} result_struct;


result_struct evaluate(map m, int debug);