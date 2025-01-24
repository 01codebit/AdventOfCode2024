#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "..\common\printer.h"

char *read_line(char *filename, int debug);

void print_list_to_file(char* filename, linked_list **plist);