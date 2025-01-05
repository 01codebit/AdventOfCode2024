#include "main.h"

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

    map m = read_map(filename);
    printf("\nread map %dx%d\n", m.cols, m.rows);

    print_matrix(m.data, m.rows, m.cols);

    antennas atns = parse_map(m, debug);

    printf("\nantennas (%d):\n", atns.count);
    for(int k=0; k<atns.count; k++)
    {
        antenna a = atns.list[k];
        printf("antenna: %c (%d, %d)\n", a.frequency, a.x, a.y);
    }

    // TODO computation

    printf("\n");
    return 0;
}
