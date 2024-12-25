#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "guard_path.h"

int main(int argc, char* argv[])
{
    // defaults
    char* filename = "example.txt";
    int debug = 0;

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];

        if(argc>2)
        {
            debug = atoi(argv[2]);
            printf("debug: %d\n", debug);
        }
    }

    printf("read file: '%s'\n", filename);
    map m = read_map(filename);
    printf("read map with rows: %d x cols: %d\n", m.rows, m.cols);

    result_struct result = evaluate(m, debug);
    printf("\nresult:\n");
    printf("guard path positions:\t%d\n", result.path_cells);
    printf("loop options:\t\t%d\n", result.loop_options);
    printf("\n");

    return 0;
}
