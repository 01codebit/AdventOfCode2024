#include "computing.h"

void insert_region(char ch, region *regions, int *regions_count)
{
    int found = 0;
    for (int i = 0; i < (*regions_count); i++)
    {
        if (regions[i].name == ch)
        {
            regions[i].area++;
            regions[i].perimeter += 4;
            found = 1;
            break;
        }
    }

    if (found==0)
    {
        region rg;
        rg.name = ch;
        rg.area = 1;
        rg.perimeter = 4;
        rg.next = NULL;

        regions[*regions_count] = rg;
        (*regions_count)++;
    }
}

void scan_matrix(data dt, region *regions, int *regions_count)
{
    printf("[scan_matrix]\n");

    for (int x = 0; x < dt.cols; x++)
    {
        for (int y = 0; y < dt.rows; y++)
        {
            printf("%c ", dt.matrix[x][y]);
            if (dt.matrix[x][y] != '.')
            {
                insert_region(dt.matrix[x][y], regions, regions_count);
            }

            // check neighbors

            dt.matrix[x][y] = '.'; // mark visited
        }
        printf("\n");
    }
}