#include "file_reader.h"

data read_matrix(char* filename)
{
    FILE* input = fopen(filename, "r");

    char ch;
    int lines = 0;
    char** result = 0;
    int len = 0;
    int cols = 0;
    char* line = 0;
    int chunks = 0;
    int chunk_size = 256;

    if(input)
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
                line = (char*)malloc(chunk_size*sizeof(char));
            }
            else if(len>chunk_size)
            {
                chunks++;
                int new_size = chunks * chunk_size;
                line = (char*)realloc(line, new_size*sizeof(char));
            }

            if(ch=='\n' || ch==EOF)
            {
                if(lines==0)
                {
                    result = (char**)malloc(sizeof(char*));
                }
                else
                {
                    result = (char**)realloc(result, len*sizeof(char*));
                }

                line[len] = '\0';
                result[lines] = line;

                if(cols==0) cols = len;

                len = -1;
                lines++;

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

    // printf("\n");
    // printf("read %d lines\n", lines);
    // for(int i=0; i<lines; i++)
    // {
    //     printf("[%d] %s\n", i, result[i]);
    // }
    // printf("\n");

    data x;
    x.matrix = result;
    x.rows = lines;
    x.cols = cols;

    return x;
}