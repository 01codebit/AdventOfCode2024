#include "file_reader.h"

map read_map(char *filename)
{
    FILE *input = fopen(filename, "r");

    char ch;
    int lines = 0;
    int **data = 0;
    int len = 0;
    int cols = 0;
    int *line = 0;
    int chunks = 0;

    if (input)
    {
        int exit = 1;
        while (exit)
        {
            ch = fgetc(input);

            if (len == 0)
            {
                if (ch == EOF)
                {
                    break;
                }
                line = (int *)malloc(chunks * CHUNK_SIZE * sizeof(int));
            }
            else if (len >= (chunks * CHUNK_SIZE))
            {
                chunks++;
                int new_size = chunks * CHUNK_SIZE;
                line = (int *)realloc(line, new_size * sizeof(int));
            }

            if (ch == '\n' || ch == EOF)
            {
                if (lines == 0)
                {
                    data = (int **)malloc(sizeof(int *));
                }
                else
                {
                    data = (int **)realloc(data, len * sizeof(int *));
                }

                data[lines] = line;

                if (cols == 0)
                    cols = len;

                len = -1;
                lines++;

                if (ch == EOF)
                    exit = 0;
            }
            else
            {
                int x = ch - '0';
                line[len] =  x;
            }

            len++;
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    if (DEBUG)
    {
        printf("\n");
        printf("[read_map] read a matrix %d lines X %d cols\n", lines, cols);
        print_int_matrix(data, lines, cols);
        printf("\n");
    }

    map m;
    m.data = (const int**) data;
    m.rows = lines;
    m.cols = cols;

    return m;
}
