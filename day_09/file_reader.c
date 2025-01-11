#include "file_reader.h"

disk_map read_disk_map(char* filename)
{
    FILE *input = fopen(filename, "r");

    int* data = (int*)malloc(CHUNK_SIZE * sizeof(int));
    int len = 0;

    char ch;

    if (input)
    {
        ch = fgetc(input);
        while(ch!=EOF)
        {
            int x = ch - '0';
            data[len] = x;
            len++;
            ch = fgetc(input);
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    disk_map m;
    m.map = data;
    m.length = len;

    return m;
}