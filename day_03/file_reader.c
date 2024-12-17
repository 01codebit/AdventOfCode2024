#include "file_reader.h"

void add_char_to_text(char* text, char c, int size)
{
    text = (char*)realloc(text, size * sizeof(char));
    text[size-1] = c;
}

data read_file(char* filename)
{
    FILE* input = fopen(filename, "r");

    data result;
    char* text = (char*)malloc(sizeof(char));
    int length = 0;

    if(input != NULL)
    {
        char ch;
        while ((ch=fgetc(input)) != EOF)
        {
            printf("%c\n", ch);
            length++;
            // add_char_to_text(text, ch, length);
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }


    result.text = text;
    result.length = length;

    return result;
}