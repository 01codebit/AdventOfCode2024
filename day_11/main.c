#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;

    char filter = ' ';

    if (argc > 1)
    {
        if (strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];

        if (argc > 2)
        {
            debug = atoi(argv[2]);
            printf("debug: %d\n", debug);
        }
    }

    printf("[main] read file: '%s'\n", filename);

    char *stones_str = read_line(filename, debug);
    if (debug)
    {
        printf("[main] read initial stones: %s\n", stones_str);
    }

    linked_list *stones = convert_to_linked_list(stones_str, debug);
    printf("[main] linked list initial length: %d\n", stones->count);
    node* n = stones->first;
    if(debug) while(n!=NULL)
    {
        printf("%s\n", n->value);
        n = n->next;
        //while(!getchar()) {}
    }

    int steps = 25;
    for (int i = 0; i < steps; i++)
    {
        blink_step(stones, debug);
        printf("[step:%2d] count: %d\n", i+1, stones->count);

        if(debug) while(!getchar()) {}
    }

    printf("\n");

    // free used memory

    return 0;
}
