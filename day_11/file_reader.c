#include "file_reader.h"

char *read_line(char *filename, int debug)
{
    FILE *input = fopen(filename, "r");

    char ch;
    int len = 0;
    int chunks = 1;
    char *line = (char *)malloc(chunks * CHUNK_SIZE * sizeof(char));

    if (input)
    {
        int exit = 1;
        while (exit)
        {
            ch = fgetc(input);

            if (len >= (chunks * CHUNK_SIZE))
            {
                chunks++;
                line = (char *)realloc(line, chunks * CHUNK_SIZE * sizeof(char));
            }

            if (ch == '\0' || ch == '\n' || ch == EOF)
            {
                ch = '\0';
                exit = 0;
            }

            line[len] = ch;
            len++;

        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    if (debug)
    {
        printf("\n");
        printf("[read_line] read a line with len %d\n", len);
        printf("[read_line] line: '%s'\n", line);
        printf("\n");
    }

    return line;
}
