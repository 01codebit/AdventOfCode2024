#include "computing.h"

int find_path(int *visited, char **matrix, const int cols, const int rows, const char ch, const int x0, const int y0, int x1, int y1)
{
    if (x0 == x1 && y0 == y1)
    {
        return 1;
    }
    else if (x1 < 0 || x1 >= rows || y1 < 0 || y0 >= cols || matrix[x1][y1] != ch || visited[x1 + y1 * rows] == 1)
    {
        return 0;
    }
    else
    {
        visited[x1 + y1 * rows] = 1;

        // for (int cc = 0; cc < cols * rows; cc++)
        // {
        //     int x = cc % cols;
        //     int y = (int)(cc / cols);
        //     printf("(%d:%d) %d ", x, y, visited[cc]);
        //     if (x == (cols - 1))
        //         printf("\n");
        // }

        return find_path(visited, matrix, cols, rows, ch, x0, y0, x1 + 1, y1) + 
            find_path(visited, matrix, cols, rows, ch, x0, y0, x1, y1 + 1) + 
            find_path(visited, matrix, cols, rows, ch, x0, y0, x1 - 1, y1) + 
            find_path(visited, matrix, cols, rows, ch, x0, y0, x1, y1 - 1);
    }
}

void insert_region(data *dt, int x, int y, region *regions, int *regions_count, int free_sides)
{
    char ch = dt->matrix[x][y];

    int found = 0;
    for (int i = 0; i < (*regions_count); i++)
    {
        if (regions[i].name == ch)
        {
            int *visited = (int *)malloc(dt->cols * dt->rows * sizeof(int));            
            for (int xx = 0; xx < dt->cols * dt->rows; xx++)
            {
                visited[xx] = 0;
            }

            // printf("[insert_region] init visited:\n");
            // for (int cc = 0; cc < dt->cols * dt->rows; cc++)
            // {
            //     int x = cc % dt->cols;
            //     int y = (int)(cc / dt->cols);
            //     printf("(%d:%d) %d ", x, y, visited[cc]);
            //     if (x == (dt->cols - 1))
            //         printf("\n");
            // }
            
            if (find_path(visited, dt->matrix, dt->cols, dt->rows, ch, regions[i].first_x, regions[i].first_y, x, y) > 0)
            {
                regions[i].area++;
                regions[i].perimeter += (4 - free_sides);
                found = 1;
            }

            free(visited);
            visited = NULL;
            
            if(found) break;
        }
    }

    if (found == 0)
    {
        region rg;
        rg.name = ch;
        rg.area = 1;
        rg.perimeter = (4 - free_sides);
        rg.next = NULL;

        rg.first_x = x;
        rg.first_y = y;

        regions[*regions_count] = rg;
        (*regions_count)++;
    }
}

void scan_matrix(data dt, region *regions, int *regions_count)
{
    int free_sides = 0;

    for (int x = 0; x < dt.rows; x++)
    {
        for (int y = 0; y < dt.cols; y++)
        {
            // printf("element (%d, %d) %c\n", y, x, dt.matrix[x][y]);
            if ((x - 1) >= 0 && dt.matrix[x - 1][y] == dt.matrix[x][y])
                free_sides++;

            if ((y - 1) >= 0 && dt.matrix[x][y - 1] == dt.matrix[x][y])
                free_sides++;

            if ((x + 1) < dt.rows && dt.matrix[x + 1][y] == dt.matrix[x][y])
                free_sides++;

            if ((y + 1) < dt.cols && dt.matrix[x][y + 1] == dt.matrix[x][y])
                free_sides++;

            // printf("[scan_matrix] call insert_region (x: %d, y: %d): [%c] free_sides: %d\n", x, y, dt.matrix[x][y], free_sides);
            insert_region(&dt, x, y, regions, regions_count, free_sides);

            free_sides = 0;
        }
        // printf("\n");
    }
}