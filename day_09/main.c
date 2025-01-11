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

    printf("read file: '%s'\n", filename);

    disk_map m = read_disk_map(filename);
    printf("\nread disk map len: %d\n", m.length);
    if (debug)
    {
        printf("\nread disk map:");
        for (int i = 0; i < m.length; i++)
        {
            printf(" %d", m.map[i]);
        }
        printf("\n");
    }

    // char *ex = expand_disk_map(m);
    expansion ex = expand_disk_map(m);
    printf("expansion length: %d\n", ex.length);
    if (debug)
    {
        printf("expansion:\n");
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }

    arrange_expansion(ex);
    if (debug)
    {
        printf("expansion:\n");
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }


    long long int checksum = compute_checksum(ex);
    printf("checksum: %lld\n", checksum);
    printf("LLONG_MAX: %lld\n", LLONG_MAX);

    char dummy[255];
    int normalInt = 5;
    printf("My number is %d bytes wide and its value is %s. A normal number is %d.\n", sizeof(checksum), lltoa(checksum, dummy, 10), normalInt);
    printf("\n");

    // free used memory
    free(ex.locations);
    free(m.map);

    return 0;
}
