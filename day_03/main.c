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
    data d = read_file(filename);

    printf("\ntext: %s\n", d.text);
    printf("\nlength: %d\n", d.length);

    int result = parse_string(d.text, d.length);
    printf("result: %d\n", result);

    printf("\n");
    return 0;
}
