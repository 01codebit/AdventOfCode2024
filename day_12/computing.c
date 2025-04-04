#include "computing.h"

void insert_region(const char ch, region *regions, int *regions_count, int free_sides)
{
    int found = 0;
    for (int i = 0; i < (*regions_count); i++)
    {
        if (regions[i].name == ch)
        {
            regions[i].area++;
            regions[i].perimeter += (4 - free_sides);
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        region rg;
        rg.name = ch;
        rg.area = 1;
        rg.perimeter = (4 - free_sides);
        rg.next = NULL;

        regions[*regions_count] = rg;
        (*regions_count)++;
    }
}

void scan_matrix(data dt, region *regions, int *regions_count)
{
    printf("[scan_matrix]\n");

    int free_sides = 0;

    for (int x = 0; x < dt.cols; x++)
    {
        for (int y = 0; y < dt.rows; y++)
        {
            char ch = dt.matrix[x][y];
            // dt.matrix[x][y] = '.';
            // printf("%c ", dt.matrix[x][y]);
            if (dt.matrix[x][y] != '.')
            {
                if ((x - 1) >= 0 && dt.matrix[x - 1][y] == ch)
                    free_sides++;

                if ((y - 1) >= 0 && dt.matrix[x][y - 1] == ch)
                    free_sides++;

                if ((x + 1) < dt.cols && dt.matrix[x + 1][y] == ch)
                    free_sides++;

                if ((y + 1) < dt.rows && dt.matrix[x][y + 1] == ch)
                    free_sides++;

                insert_region(ch, regions, regions_count, free_sides);

                free_sides = 0;
            }
        }
        // printf("\n");
    }
}