#include "file_reader.h"

char* read_text(char* filename)
{
    FILE* input = fopen(filename, "r");

    char* text = 0;
    long length;

    if(input)
    {
        fseek(input, 0, SEEK_END);
        length = ftell(input);
        fseek(input, 0, SEEK_SET);
        text = (char*)malloc(length*sizeof(char));
        if(text)
        {
            fread(text, 1, length, input);
        }
        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }

    return text;
}