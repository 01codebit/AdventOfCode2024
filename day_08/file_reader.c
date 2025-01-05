#include "file_reader.h"

map read_map(char *filename)
{
    FILE *input = fopen(filename, "r");

    char** data = 0;
    char* line = 0;
    int cols_count = 0;
    int rows_count = 0;
    
    int chunks = 0;
    char ch;
    int len = 0;

    if (input)
    {
        int exit = 1;
        while(exit)
        {
            ch = fgetc(input);
            // printf("lines: %d, len: %d\n", lines, len);

            if(len==0)
            {
                if(ch==EOF)
                {
                    break;
                }
                line = (char*)malloc(CHUNK_SIZE*sizeof(char));
            }
            else if(len>CHUNK_SIZE)
            {
                chunks++;
                int new_size = chunks * CHUNK_SIZE;
                line = (char*)realloc(line, new_size*sizeof(char));
            }

            if(ch=='\n' || ch==EOF)
            {
                if(rows_count==0)
                {
                    data = (char**)malloc(sizeof(char*));
                }
                else
                {
                    data = (char**)realloc(data, len*sizeof(char*));
                }

                line[len] = '\0';
                data[rows_count] = line;

                if(cols_count==0) cols_count = len;

                len = -1;
                rows_count++;

                if(ch==EOF) exit = 0;
            }
            else
            {
                line[len] = ch;
            }

            len++;
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }


    map m;
    m.data = data;
    m.cols = cols_count;
    m.rows = rows_count;

    return m;
}