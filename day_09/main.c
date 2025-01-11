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

    char *ex = expand_disk_map(m);
    printf("expansion length: %d\n", strlen(ex));
    if (debug)
        printf("expansion: %s\n", ex);

    arrange_expansion(ex);
    if (debug)
        printf("arranged expansion: %s\n", ex);

    long int checksum = compute_checksum(ex);
    printf("checksum: %ld\n", checksum);

    printf("\n");
    return 0;
}
