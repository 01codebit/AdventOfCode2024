#include "file_reader.h"

void add_char_to_text(char* text, char c, int size)
{
}

data read_file(char* filename)
{
    FILE* input = fopen(filename, "r");

    data result;
    char* text;
    int count = 0;

    if(input != NULL)
    {
        char ch;
        while ((ch=fgetc(input)) != EOF)
        {
            printf("[%d] %c\n", count, ch);
            if(count==0)
            {
                text = (char*)malloc(sizeof(char));
            }
            else
            {
                text = (char*)realloc(text, count * sizeof(char));
            }
            text[count] = ch;
            count++;
            // add_char_to_text(text, ch, length);
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }


    result.text = text;
    result.length = count;

    return result;
}


char* read_text(char* filename)
{
    FILE* input = fopen(filename, "r");

    char* line = (char*)malloc(LINE_LEN*sizeof(char));
    char* text = (char*)malloc(1024*sizeof(char));

    // memset(text, 0, 1024*sizeof(char));

    int count = 0;

    if(input != NULL)
    {
        char ch;
        while (fgets(line, LINE_LEN, input))
        {
            printf("%s\n", line);
            strcmp(text, line);
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }

    // printf("text: %s\n", text);

    return text;
}