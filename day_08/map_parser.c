#include "map_parser.h"

antennas parse_map(map m, int debug)
{
    int count = 0;
    int chunks = 1;
    antenna *list = (antenna *)malloc(chunks * CHUNK_SIZE * sizeof(antenna));
    char ch;

    for (int y = 0; y < m.rows; y++)
    {
        for (int x = 0; x < m.cols; x++)
        {
            ch = m.data[y][x];

            if (ch != '.')
            {
                antenna atn;
                atn.id = count;
                atn.frequency = ch;
                atn.x = x;
                atn.y = y;

                list[count] = atn;
                count++;

                if (count > (CHUNK_SIZE * chunks))
                {
                    chunks++;
                    printf("  [map_parser.parse_map][realloc] new size: %d\n", chunks * CHUNK_SIZE);
                    antenna *new_list = (antenna *)realloc(list, (chunks * CHUNK_SIZE) * sizeof(antenna));
                    list = new_list;
                }
            }
        }
    }

    antennas atns;
    atns.list = list;
    atns.count = count;

    return atns;
}

map filter_map(map m, char filter)
{
    char ch;

    for (int y = 0; y < m.rows; y++)
    {
        for (int x = 0; x < m.cols; x++)
        {
            ch = m.data[y][x];
            if (ch != '.' && ch != filter)
            {
                m.data[y][x] = '.';
            }
        }
    }
    
    return m;
}