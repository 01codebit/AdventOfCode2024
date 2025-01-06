#include "computing.h"

antinodes antinodes_count(map m, antennas atns, int debug)
{
    debug = 0;

    int count = -1;

    int chunks = 1;
    antinode *ans = (antinode *)malloc(CHUNK_SIZE * sizeof(antinode));

    for (int i = 0; i < atns.count; i++)
    {
        antenna start = atns.list[i];
        // if (debug)
        //     printf("i=%d start: #%d '%c' (%d, %d)\n", i, start.id, start.frequency, start.x, start.y);

        for (int j = 0; j < atns.count; j++)
        {
            if (j == i)
                continue;

            antenna end = atns.list[j];
            if (debug)
                printf("j=%d end: #%d '%c' (%d, %d)\n", j, end.id, end.frequency, end.x, end.y);

            if (start.frequency == end.frequency && start.id != end.id)
            {
                if (debug)
                    printf("  find_antinode\n");
                antinode an = find_antinode(start, end);

                if (an.x < m.cols && an.x >= 0 && an.y < m.rows && an.y >= 0)
                {
                    if (m.data[an.y][an.x] == '.')
                    {
                        count++;

                        if (count > (CHUNK_SIZE * chunks))
                        {
                            chunks++;
                            printf("  [computing.antinodes_count][realloc] new size: %d\n", chunks * CHUNK_SIZE);

                            antinode *new_ans = (antinode *)realloc(ans, chunks * CHUNK_SIZE * sizeof(antinode));
                            ans = new_ans;
                        }

                        ans[count] = an;
                        if (debug)
                            printf(" ---> antinode #%d (%c: %d, %d) ", count, an.frequency, an.x, an.y);
                    }
                    else
                    {
                        if (debug)
                            printf(" CELL NOT FREE: antinode(%c: %d, %d) ", an.frequency, an.x, an.y);
                    }
                }
                else
                {
                    if (debug)
                        printf(" OUT OF BOUNDS: antinode(%c: %d, %d) ", an.frequency, an.x, an.y);
                }

                if (debug)
                    printf("\n");
            }
        }
    }

    antinodes res;
    res.list = ans;
    res.count = count + 1;

    return res;
}