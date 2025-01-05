#include "map_parser.h"

antennas parse_map(map m, int debug)
{
    int count = 0;
    antenna list[100];
    char ch;

    for (int y = 0; y < m.rows; y++)
    {
        for (int x = 0; x < m.cols; x++)
        {
            ch = m.data[y][x];

            if (ch != '.')
            {
                antenna atn;
                atn.frequency = ch;
                atn.x = x;
                atn.y = y;

                list[count] = atn;
                count++;
            }
        }
    }

    antennas atns;
    atns.list = list;
    atns.count = count;

    return atns;
}