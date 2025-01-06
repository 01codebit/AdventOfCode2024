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

        if (argc > 3)
        {
            filter = argv[3][0];
            printf("filter: %d\n", filter);
        }
    }

    printf("read file: '%s'\n", filename);

    map m = read_map(filename);
    printf("\nread map %dx%d\n", m.cols, m.rows);

    if (debug)
    {
        print_matrix(m.data, m.rows, m.cols);
    }

    
    if(filter!=' ')
    {
        m = filter_map(m, filter);
    }

    antennas atns = parse_map(m, debug);
    printf("\nfound %d antennas\n", atns.count);

    char frequencies[1024];
    int counts[1024];
    int groups = 0;
    for (int k = 0; k < atns.count; k++)
    {
        antenna a = atns.list[k];
        if (debug)
            printf("#%d: %c (%d, %d)\n", a.id, a.frequency, a.x, a.y);

        int found = 0;
        for (int o = 0; o < groups; o++)
        {
            if (frequencies[o] == a.frequency)
            {
                counts[o]++;
                found = 1;
            }
        }
        if (!found)
        {
            frequencies[groups] = a.frequency;
            counts[groups] = 1;
            groups++;
        }
    }

    int total_combinations = 0;
    for (int o = 0; o < groups; o++)
    {
        int ss = sigma(counts[o]) * 2;
        total_combinations += ss;
        if (debug)
            printf("gruppo '%c' nodi: %d -> combinazioni: %d\n", frequencies[o], counts[o], ss);
    }
    printf("total combinations: %d\n", total_combinations);

    antinodes ans = antinodes_count(m, atns, debug);
    printf("\nantinodes count: %d\n", ans.count);

    if (debug)
    {
        int anodes[1024];
        for (int i = 0; i < 1024; i++)
            anodes[i] = 0;
        for (int k = 0; k < ans.count; k++)
        {
            char fr = ans.list[k].frequency;

            if (fr == filter)
                printf("  node: %c (%d, %d)\n", fr, ans.list[k].x, ans.list[k].y);

            for (int h = 0; h < groups; h++)
            {
                if (fr == frequencies[h])
                {
                    anodes[h]++;
                    break;
                }
            }
        }

        for (int h = 0; h < groups; h++)
        {
            printf("[%d] fr: '%c' - nodes: %d --> antinodes: %d\n", h, frequencies[h], counts[h], anodes[h]);
        }
    }

    if (debug)
    {
        int ans_x[1024];
        int ans_y[1024];
        for (int i = 0; i < ans.count; i++)
        {
            ans_x[i] = ans.list[i].x;
            ans_y[i] = ans.list[i].y;
        }

        print_matrix_markers(m.data, m.rows, m.cols, '#', ans_x, ans_y, ans.count);
    }

    printf("\n");
    return 0;
}
