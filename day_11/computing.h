#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "data.h"

linked_list *convert_to_linked_list(const char *str, int debug);

char *blink_step_str(const char *stones, int debug);

void blink_step(linked_list *ll, int debug);