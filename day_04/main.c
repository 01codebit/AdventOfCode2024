#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "word_searcher.h"

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
    data t = read_matrix(filename);
    printf("\nmatrix rows:%d x cols:%d\n", t.rows, t.cols);

    int result = count_occurrences(t.matrix, t.rows, t.cols, "XMAS");

    printf("result: %d\n", result);

    printf("\n");
    return 0;
}
