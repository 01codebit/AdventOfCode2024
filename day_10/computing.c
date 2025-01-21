#include "computing.h"

int find_trail(const map m, const int x, const int y, const int value, int *check_list)
{
    int debug = 0;

    if (x < 0 || x >= m.cols || y < 0 || y >= m.rows)
    {
        return 0;
    }

    int target = value + 1;

    if (m.data[y][x] == target)
    {
        if(debug) printf("[find_trail] step [%d] in (%d, %d)", target, x, y);

        if (target == 9)
        {
            if(debug) printf(" [FOUND trail end]\n");
            //while (!getchar()){};

            // part 1: trailhead score
            if(PART == 1)
            {
                if(check_list[y * m.rows + x]==0)
                {
                    check_list[y * m.rows + x] = 1;
                    return 1;
                }
                else return 0;
            }

            // part 2: rating
            if(PART == 2) return 1;
        }
        else
        {
            if(debug) printf("\n");
            // printf("[find_trail] search around (%d, %d)\n", x, y);
            return find_trail(m, x - 1, y, target, check_list) +
                   find_trail(m, x, y - 1, target, check_list) +
                   find_trail(m, x + 1, y, target, check_list) +
                   find_trail(m, x, y + 1, target, check_list);
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
    // printf("[search] start search from (%d, %d)\n", x, y);

    int *check_list = (int *)malloc(m.cols * m.rows * sizeof(int));
    for(int i=0; i<m.cols * m.rows; i++)
    {
        check_list[i]=0;
    };

    return find_trail(m, x - 1, y, target, check_list) +
           find_trail(m, x, y - 1, target, check_list) +
           find_trail(m, x + 1, y, target, check_list) +
           find_trail(m, x, y + 1, target, check_list);
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
                result += search(m, x, y);
            }
        }
    }

    return result;
}