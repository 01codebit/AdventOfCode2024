#include "file_reader.h"

disk_map read_disk_map(char *filename)
{
    FILE *input = fopen(filename, "r");

    int *data = (int *)malloc(CHUNK_SIZE * sizeof(int));
    if (data == NULL)
    {
        printf("[read_disk_map] Cannot allocate %zu bytes for data array\n", CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    int len = 0;
    int chunks = 1;

    char ch;

    if (input)
    {
        ch = fgetc(input);
        while (ch != EOF)
        {
            int x = ch - '0';

            if (len > CHUNK_SIZE * chunks)
            {
                chunks++;
                data = (int *)realloc(data, chunks * CHUNK_SIZE * sizeof(int));
                if (data == NULL)
                {
                    printf("[read_disk_map] Cannot allocate %zu bytes for data array\n", chunks * CHUNK_SIZE);
                    exit(EXIT_FAILURE);
                }
            }

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

    printf("[read_disk_map] result length: %d\n", len);

    return m;
}