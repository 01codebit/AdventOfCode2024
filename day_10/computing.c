#include "computing.h"

int find_trail(const map m, const int x, const int y, const int value)
{
    int debug = 0;

    if (x < 0 || x >= m.cols || y < 0 || y >= m.rows)
    {
        return 0;
    }

    int target = value + 1;

    if (m.data[y][x] == target)
    {
        printf("[find_trail] step [%d] in (%d, %d)", target, x, y);

        if (target == 9)
        {
            printf(" [FOUND trail end]\n");
            while (!getchar()){};

            return 1;
        }
        else
        {
            printf("\n");
            //printf("[find_trail] search around (%d, %d)\n", x, y);
            return find_trail(m, x - 1, y, target) +
                   find_trail(m, x, y - 1, target) +
                   find_trail(m, x + 1, y, target) +
                   find_trail(m, x, y + 1, target);
        }
    }
    else
    {
        // printf("[find_trail] found nothing (%d, %d)\n", x, y);
        return 0;
    }
}

int search(const map m, const int x, const int y)
{
    int target = 0;
    printf("[search] start search from (%d, %d)\n", x, y);

    return find_trail(m, x - 1, y, target) +
           find_trail(m, x, y - 1, target) +
           find_trail(m, x + 1, y, target) +
           find_trail(m, x, y + 1, target);
}

int compute_trails(const map m)
{
    int result = 0;

    for (int y = 0; y < m.rows; y++)
    {
        for (int x = 0; x < m.rows; x++)
        {
            int current = m.data[y][x];

            if (current == 0)
            {
                // printf("found trailhead in (%d, %d)\n", x, y);
                //  trailhead
                result = search(m, x, y);
            }
        }
    }

    return result;
}