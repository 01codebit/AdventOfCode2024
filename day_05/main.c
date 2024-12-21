#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"

int main(int argc, char* argv[])
{
    // defaults
    char* filename = "example.txt";

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];
    }

    printf("read file: '%s'\n", filename);
    data t = read_data(filename);

    int rules_count = sizeof(t.rules) / sizeof(rule);
    printf("main ----------------------------------------------------------------\n");
    for(int i=0; i<rules_count; i++)
    {
        printf("rule[%d] prev: %d - next: %d\n", i, t.rules[i].prev, t.rules[i].next);
    }

    printf("\n");
    return 0;
}
