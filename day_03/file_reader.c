#include "file_reader.h"

int get_file_length(FILE* f)
{
    if(fseek(f, 0, SEEK_END))
    {
        fprintf(stderr, "[fseek] error: %s [errno:%d]\n", strerror(errno), errno);
    }
    long length = ftell(f);
    if(fseek(f, 0, SEEK_SET))
    {
        fprintf(stderr, "[fseek] error: %s [errno:%d]\n", strerror(errno), errno);
    }

    return length;
}

char* read_text(char* filename)
{
    FILE* input = fopen(filename, "r");

    char* text = 0;
    long length;

    if(input)
    {
        length = get_file_length(input);
        text = (char*)malloc(length * sizeof(char));
        
        if(text)
        {
            fread(text, sizeof(char), length, input);
        }
        fclose(input);
    }
    else
    {
       fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }
    text[length] = '\0';

    return text;
}