#include "utility.h"

int sigma(int max)
{
    if(max == 0) return 0;
    else
        return max - 1 + sigma(max - 1);
}


antinode find_antinode(antenna start, antenna end)
{
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    int nx = end.x + dx;
    int ny = end.y + dy;

    antinode an;
    an.frequency = end.frequency;
    an.x = nx;
    an.y = ny;

    // printf("new antinode: %c (%d, %d)\n", an.frequency, an.x, an.y);

    return an;
}