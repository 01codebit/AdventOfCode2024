#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "safe_calculator.h"

int main(int argc, char* argv[])
{
    // defaults
    char* filename = "example.txt";
    int debug = 0;

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename> <int print_result (0|1) (default:0)>\n\n");
            return 0;
        }
        filename = argv[1];

        if(argc==3)
        {
            debug = atoi(argv[2]);
        }
    }

    printf("read file: '%s'\n", filename);
    data d = read_file(filename);

    printf("\nnumber of reports: %d\n", d.reports_count);

    if(debug)
    {
        printf("-----------------------------------\n");
        for(int r=0; r<d.reports_count; r++)
        {
            for(int l=0; l<d.levels_counts[r]; l++)
            {
                printf("%d ", d.array[r][l]);
            }
            printf("\n");
        }
        printf("-----------------------------------\n");
    }

    int safe_count = total_safe_count(d.array, d.reports_count, d.levels_counts);
    printf("\nsafe count: %d\n", safe_count);

    printf("\n");
    return 0;
}
