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
        if(debug) printf("[read_line] reading '%s'\n", filename);
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
        printf("[read_line] read a line with len %d\n", len);
        printf("[read_line] line: '%s'\n", line);
    }

    return line;
}


void print_list_to_file(char* filename, linked_list **plist)
{
    FILE *output = fopen(filename, "w");
    if(!output)
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
        return;
    }

    linked_list *list = *plist;
    node *current_node = list->first;
    // printf("[print_list_to_file] linked list length: %d print to file '%s'\n", list->count, filename);

    int count = 0;
    while (current_node != NULL)
    {
        // printf("[%d] %lld ", count, current_node->value);
        fprintf(output, "%lld", current_node->value);
        current_node = current_node->next;
        count++;
        if (current_node != NULL)
        {
            // printf("-> ");
            fprintf(output, " ");
        }
    }
    
    fclose(output);
}