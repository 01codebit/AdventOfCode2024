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

    ULLONG checksum = arrange_expansion(ex);
    if (debug)
    {
        printf("[main] arranged expansion (length: %d):\n", ex.length);
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }

    // ULLONG checksum = compute_checksum(ex);
    printf("\n[main] checksum: %lld\n", checksum);
    printf("ERROR: %lld\n", 6283170117911-checksum);
    // printf("LLONG_MAX: %lld\n", LLONG_MAX);

    // char dummy[255];
    // int normalInt = 5;
    // printf("My number is %d bytes wide and its value is %s. A normal number is %d.\n", sizeof(checksum), lltoa(checksum, dummy, 10), normalInt);
    printf("\n");

    // free used memory
    free(ex.locations);
    free(m.map);

    return 0;
}
