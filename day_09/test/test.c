#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_function(int n)
{
    int chunk_size = 8;
    int chunks = 1;
    int len = 0;

    char *text = (char *)malloc(chunks * chunk_size * sizeof(char));
    memset(text, 0, chunk_size * chunks);
    printf("strlen: %u\n", strlen(text));
    printf("text: %s\n", text);

    if (text == NULL)
    {
        printf("error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
    {
        if (len > chunks * chunk_size)
        {
            chunks++;
            text = (char *)realloc(text, chunks * chunk_size * sizeof(char));
            if (text == NULL)
            {
                printf("error reallocating memory\n");
                exit(EXIT_FAILURE);
            }
        }

        text[i] = 'A';

        len++;
    }

    text[len] = 0;

    printf("strlen: %u\n", strlen(text));
    printf("len: %u\n", len);
    printf("text: %s\n", text);

    return text;
}

int main(int argc, char *argv[])
{

    char *text = str_function(1000);

    printf("[main] text: %s\n", text);

    free(text);

    return 0;
}
