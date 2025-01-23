#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

void print_linked_list(linked_list **plist)
{
    linked_list *list = *plist;
    node *current_node = list->first;
    printf("[print_linked_list] linked list length: %d\n", list->count);

    int count = 0;
    while (current_node != NULL)
    {
        printf("[%d] %lld ", count, current_node->value);
        current_node = current_node->next;
        count++;
        if (current_node != NULL)
            printf("-> ");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;
    int steps = 25;

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
            int a2 = atoi(argv[2]);
            if(a2>1) 
            {
                steps = a2;
                printf("steps: %d\n", steps);
            }
            else
            {
                debug = atoi(argv[2]);
                printf("debug: %d\n", debug);
            }
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
    node *n = stones->first;
    if (debug)
        while (n != NULL)
        {
            printf("%lld\n", n->value);
            n = n->next;
            // while(!getchar()) {}
        }
        
    print_linked_list(&stones);

    for (int i = 0; i < steps; i++)
    {
        blink_step(&stones, debug);
        printf("[step:%2d] count: %lld\n", i + 1, stones->count);
        if(debug) print_linked_list(&stones);

        if (debug)
            while (!getchar())
            {
            }
    }

    printf("\n");

    // free used memory

    return 0;
}
