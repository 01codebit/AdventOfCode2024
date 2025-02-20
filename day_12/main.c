#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main(int argc, char* argv[])
{
    struct timeval t0;
    struct timeval t1;

    // defaults
    char* filename = "example_1.txt";

    if(argc>1)
    {
        if(strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];
    }

    printf("[main] read file: '%s'\n", filename);
    data t = read_matrix(filename);
    printf("[main] matrix rows:%d x cols:%d\n", t.rows, t.cols);

    gettimeofday(&t0, 0);

    region regions[100];
    int regions_count = 0;
    scan_matrix(t, regions, &regions_count);

    int total_cost = 0;
    int region_cost = 0;
    for(int i=0; i<regions_count; i++)
    {
        printf("[main] region: %c area: %d perimeter: %d\n", regions[i].name, regions[i].area, regions[i].perimeter);
        region_cost = regions[i].area * regions[i].perimeter;
        total_cost += region_cost;
    }

    gettimeofday(&t1, 0);
    float elapsed = timedifference_msec(t0, t1);

    printf("[main] total cost: %d\n", total_cost);
    printf("[main] elapsed time = %f ms\n\n", elapsed);

    return 0;
}
