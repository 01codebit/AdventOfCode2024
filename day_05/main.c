#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "checker.h"

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
    printf("read %d rules for %d updates\n", t.rules_count, t.updates_count);

    /*
    printf("main ----------------------------------------------------------------\n");
    for(int i=0; i<t.rules_count; i++)
    {
        printf("rule[%d] prev: %d - next: %d\n", i, t.rules[i].prev, t.rules[i].next);
    }
    for(int i=0; i<t.updates_count; i++)
    {
        update current_update = t.updates[i];
        printf("update[%d]:", i);
        for(int j=0; j<current_update.size; j++)
        {
            printf(" %d", current_update.chapters[j]);
        }
        printf("\n");
    }
    */

    int result = evaluate(t);

    printf("result: %d\n", result);

    printf("\n");
    return 0;
}
