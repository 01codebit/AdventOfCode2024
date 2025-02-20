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
        if (debug)
            printf("[read_line] reading '%s'\n", filename);
        if (debug)
            printf("[read_line] reading '%s'\n", filename);
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

void print_list_to_file(char *filename, node *list, long long nodes_count)
void print_list_to_file(char *filename, node **plist, long long nodes_count)
{
    printf("[print_list_to_file] filename: '%s', nodes count: %lld\n", filename, nodes_count);
    FILE *output = fopen(filename, "w");
    if (!output)
    if (!output)
    {
        fprintf(stderr, "[print_list_to_file][fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
        return;
    }

    long long int zeros_count = 0;
    long long int ones_count = 0;

    node *list = *plist;
    // printf("[print_list_to_file] linked list length: %d print to file '%s'\n", list->count, filename);
    for (long long int i = 0; i < nodes_count; i++)
    {

    if (nodes_count > 100)
    {
        printf("[print_list_to_file] last node: ");
        printf("node[%lld] = %lld\n", nodes_count - 1, list[nodes_count - 1]);
    }

    for (long long i = 0; i < nodes_count; i++)
    {
        node current_node = list[i];

        fprintf(output, "%lld", current_node.value);
        if (current_node.value == 0)
            zeros_count++;
        if (current_node.value == 1)
            ones_count++;
        
        // printf("[%lld/%lld] %lld\n", i, nodes_count, current_node.value);
        
        if (i != nodes_count - 1)
        // if(nodes_count>100)
        // {
        //     printf("[%d] %lld\n", i, current_node.value);
        //     continue;
        // }

        int r = fprintf(output, "%lld", current_node.value);
        if (r < 0)
        {
            perror("[print_list_to_file][perror] fprintf error\n");
            printf("[print_list_to_file] fprintf error: %d\n", r);
            break;
        }

        // printf("[%lld] %lld\n", i, current_node.value);
        if (i != nodes_count - 1)
        {
            int r = fprintf(output, " ");
            if (r < 0)
            {
                perror("[print_list_to_file][perror] fprintf error\n");
                printf("[print_list_to_file] fprintf error: %d\n", r);
                break;
            }
        }
    }
    // printf("...\n");

    printf("[print_list_to_file] zeroes: %lld, ones: %lld\n", zeros_count, ones_count);


    printf("[print_list_to_file] closing file '%s'\n", filename);
    fclose(output);
}