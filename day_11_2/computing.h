#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "data.h"

char *get_time_string(float time);

void print_nodes_array(node *nodes, long long count);

long long convert_to_node_array(node *nodes, char *str, int debug);

long long compute_n_steps(node *nodes, long long start_node_count, int steps, int debug, FILE *log_file);