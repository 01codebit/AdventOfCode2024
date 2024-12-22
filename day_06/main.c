#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "guard_path.h"

int main(int argc, char* argv[])
{
    // defaults
    char* filename = "example.txt";

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];
    }

    printf("read file: '%s'\n", filename);
    map m = read_map(filename);
    printf("read map with rows: %d x cols: %d\n", m.rows, m.cols);

    int result = evaluate(m);
    printf("guard path positions: %d\n", result);

    printf("\n");

    return 0;
}
