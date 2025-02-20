#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;
    int steps = 1;

    time_t start_t, end_t;
    double diff_t;

    struct timeval t0;
    struct timeval t1;

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
            if (argv[2][0] == 's')
            {
                argv[2][0] = '0';
                int a2 = atoi(argv[2]);
                steps = a2;
            }
            else if (argv[2][0] == 'd')
            {
                debug = 1;
            }
        }
        if (argc == 4)
        {
            if (argv[3][0] == 's')
            {
                argv[3][0] = '0';
                int a3 = atoi(argv[3]);
                steps = a3;
            }
            else if (argv[3][0] == 'd')
            {
                debug = 1;
            }
        }
    }
    printf("[main] filename: '%s', steps: %d, debug: %d\n", filename, steps, debug);

    FILE *log_file = fopen("log.txt", "w");
    if (!log_file)
    {
        printf("ERROR opening log file!\n");
    }

    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    fprintf(log_file, "Current local time and date: %s\n", asctime(info));
    fprintf(log_file, "Input file: '%s'\n", filename);
    fprintf(log_file, "Steps: %d\n\n", steps);

    // read initial list
    char *list = read_line(filename, debug);
    printf("[main] read line: %s\n", list);

    long long chunks = 1;
    long long int *values = (long long int *)malloc(chunks * CHUNK_SIZE * sizeof(long long int));
    long long nodes_count = convert_to_values_array(values, list, chunks, debug);

    printf("[main] initial nodes count: %lld\n", nodes_count);

    gettimeofday(&t0, 0);

    long long total_count = compute_depth_r(values, nodes_count, steps);

    gettimeofday(&t1, 0);
    float elapsed = timedifference_msec(t0, t1);

    // results output ------------------------------------------------------------------
    printf("[main] nodes count after %d steps: %lld\n", steps, total_count);
    printf("[main] elapsed time = %f ms\n\n", elapsed);

    fprintf(log_file, "Step %d: nodes count is %lld\n\n", steps, total_count);
    fprintf(log_file, "Elapsed time = %f ms\n", elapsed);
    // results output ------------------------------------------------------------------

    // close used files
    fclose(log_file);

    // free used memory
    free(values);

    return 0;
}
