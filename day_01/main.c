#include <stdio.h>
#include <stdlib.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "distance.h"
#include "similarity.h"

int main(int argc, char* argv[])
{
    // defaults
    char* filename = "input_test.txt";
    int debug = 0;

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename> <int print_result (0|1) (default:0)>\n\n");
            return 0;
        }
        filename = argv[1];

        if(argc==3)
        {
            debug = atoi(argv[2]);
        }
    }

    printf("read file: '%s'\n", filename);
    data d = read_file(filename);

    if(debug)
    {
        printf("\n-----------------------------------\n");
        printf("read %d couples of values from file:\n", d.size);
        printf("-----------------------------------\n");
        for(int i=0; i<d.size; i++)
        {
            printf("%d %d\n", d.array1[i], d.array2[i]);
        }
        printf("-----------------------------------\n");
    }
    else
    {
        printf("\nread %d couples of values from file\n", d.size);
    }

    order_list(d.array1, d.size);
    if(debug) print_array("ordered first list", d.array1, d.size);

    order_list(d.array2, d.size);
    if(debug) print_array("ordered second list", d.array2, d.size);


    // #1 --------------------------------------------------------------
    int* delta_array = distance_sum_array(d.array1, d.array2, d.size);
    if(debug) print_array("delta list", delta_array, d.size);
    printf("\ntotal delta: %d\n", array_sum(delta_array, d.size));
    // -----------------------------------------------------------------

    // #2 --------------------------------------------------------------
    int similarity = similarity_score(d.array1, d.array2, d.size);
    printf("\nsimilarity score: %d\n", similarity);
    // -----------------------------------------------------------------

    printf("\n");
    return 0;
}
