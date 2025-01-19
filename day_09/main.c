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

    disk_map m = read_disk_map(filename);
    if (debug)
    {
        printf("[main] read disk map (length: %d):\n", m.length);
        for (int i = 0; i < m.length; i++)
        {
            printf(" %d", m.map[i]);
        }
        printf("\n");
    }

    expansion ex = expand_disk_map(m);
    if (debug)
    {
        printf("[main] expansion (length: %d):\n", ex.length);
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }

    int expanded_free_count = 0;
    FILE *output = fopen("expanded.txt", "w");
    if (output == NULL)
        printf("[main] ERROR opening file expanded.txt\n");
    else
    {
        for (int i = 0; i < ex.length; i++)
        {
            fprintf(output, " %d", ex.locations[i]);
            if (ex.locations[i] < 0)
                expanded_free_count++;
        }
        fprintf(output, "\n");
        fclose(output);
    }

    printf("[main] expanded_free_count: %d\n", expanded_free_count);

    const int part = 2;

    // PART 1 -------------------------------------------------------- begin
    if (part == 1)
    {
        LLONG checksum_1 = arrange_expansion(ex);
        printf("\n[main] checksum_1: %lld\n", checksum_1);
        if (debug)
        {
            printf("[main] arranged expansion (length: %d):\n", ex.length);
            for (int i = 0; i < ex.length; i++)
            {
                printf(" %d", ex.locations[i]);
            }
            printf("\n");
        }
    }
    // PART 1 -------------------------------------------------------- end

    // PART 2 -------------------------------------------------------- begin
    if (part == 2)
    {
        arrange_expansion_n(ex);
        if (debug)
        {
            printf("[main] arranged expansion (length: %d):\n", ex.length);
            for (int i = 0; i < ex.length; i++)
            {
                printf(" %d", ex.locations[i]);
            }
            printf("\n");
        }
        // if (debug)
        {
            int arranged_free_count = 0;
            FILE *output = fopen("arranged.txt", "w");
            if (output == NULL)
                printf("[main] ERROR opening file arranged.txt\n");
            else
            {
                for (int i = 0; i < ex.length; i++)
                {
                    fprintf(output, " %d", ex.locations[i]);
                    if (ex.locations[i] < 0)
                        arranged_free_count++;
                }
                fprintf(output, "\n");
                fclose(output);
            }
            printf("[main] arranged_free_count: %d\n", arranged_free_count);
        }
    }
    // PART 2 -------------------------------------------------------- end

    LLONG checksum = compute_checksum(ex);
    printf("\n[main] checksum: %lld\n", checksum);

    printf("\n");

    // free used memory
    free(ex.locations);
    free(m.map);

    return 0;
}
