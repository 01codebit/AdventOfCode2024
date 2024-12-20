#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "parser.h"

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
    char* t = read_text(filename);
    //printf("\nreturned text: %s\n", t);

    int result = parse_string(t);

    printf("result: %d\n", result);

    printf("\n");
    return 0;
}
