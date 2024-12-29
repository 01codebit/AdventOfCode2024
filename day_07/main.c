#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;

    if (argc > 1)
    {
        if (strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];

        if (argc > 2)
        {
            debug = atoi(argv[2]);
            printf("debug: %d\n", debug);
        }
    }

    printf("read file: '%s'\n", filename);

    calibrations m = read_calibrations(filename);
    printf("\nread %d calibrations\n", m.count);

    if (debug)
    {
        printf("\ncalibrations:\n");
        for (int i = 0; i < m.count; i++)
        {
            printf("%d: (%d) ", m.data[i].total, m.data[i].count);
            for (int j = 0; j < m.data[i].count; j++)
            {
                printf("%d ", m.data[i].numbers[j]);
            }
            printf("\n");
        }
    }
    // printf("\nresult:\n");
    // printf("result:\t\t%d\n", result);
    printf("\n");

    return 0;
}
