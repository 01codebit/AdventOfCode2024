#include "main.h"

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

    // char *ex = expand_disk_map(m);
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
    // if (debug)
    {
        FILE *output = fopen("expanded.txt", "w");
        if (output == NULL)
            printf("[main] ERROR opening file expanded.txt\n");
        else
        {
            for (int i = 0; i < ex.length; i++)
            {
                // if(ex.locations[i]<0)
                //     fprintf(output, ".,");
                // else
                //     fprintf(output, "%d,", ex.locations[i]);
                fprintf(output, " %d", ex.locations[i]);
            }
            fprintf(output, "\n");
            fclose(output);
        }
    }

    // PART 1 -------------------------------------------------------- begin
    // ULLONG checksum_1 = arrange_expansion(ex);
    // printf("\n[main] checksum_1: %lld\n", checksum_1);
    // if (debug)
    // {
    //     printf("[main] arranged expansion (length: %d):\n", ex.length);
    //     for (int i = 0; i < ex.length; i++)
    //     {
    //         printf(" %d", ex.locations[i]);
    //     }
    //     printf("\n");
    // }
    // PART 1 -------------------------------------------------------- end

    // PART 2 -------------------------------------------------------- begin
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
        FILE *output = fopen("arranged.txt", "w");
        if (output == NULL)
            printf("[main] ERROR opening file arranged.txt\n");
        else
        {
            for (int i = 0; i < ex.length; i++)
            {
                fprintf(output, " %d", ex.locations[i]);
            }
            fprintf(output, "\n");
            fclose(output);
        }
    }
    // PART 2 -------------------------------------------------------- end

    ULLONG checksum = compute_checksum(ex);
    printf("\n[main] checksum: %lld", checksum);

    if (checksum >= 6307653502443)
        printf(" <--- ERROR: checksum is too high!\n"); // first try result

    printf("\n");

    // free used memory
    free(ex.locations);
    free(m.map);

    return 0;
}
