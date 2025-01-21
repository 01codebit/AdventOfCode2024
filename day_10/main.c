#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;

    char filter = ' ';

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

    printf("[main] read file: '%s'\n", filename);

    map m = read_map(filename);
    if (debug)
    {
        printf("[main] read map cols:%d x rows:%d:\n", m.cols, m.rows);
        for (int y = 0; y < m.rows; y++)
        {
            for (int x = 0; x < m.cols; x++)
            {
                printf(" %d", m.data[y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }

    int trails = compute_trails(m);

    printf("[main] computed trails: %d\n", trails);

    printf("\n");

    // free used memory
    free(m.data);

    return 0;
}
