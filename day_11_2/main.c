#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;
    int steps = 1;

    char filter = ' ';

    time_t start_t, end_t;
    double diff_t;

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

    time(&start_t);

    // read initial list
    char *list = read_line(filename, debug);
    printf("[main] read line: %s\n", list);

    long long chunks = 1;
    node *nodes = (node *)malloc(chunks * CHUNK_SIZE * sizeof(node));
    long long nodes_count = convert_to_node_array(nodes, list, chunks, debug);
    // if(debug)
    printf("[main] initial nodes count: %lld\n", nodes_count);
    if (debug)
        print_nodes_array(nodes, nodes_count);

    long long list_count = 0;
    const char *filename_format = "output/list_%lld.txt";
    char llist_filename[30];
    sprintf(llist_filename, filename_format, list_count);
    print_list_to_file(llist_filename, nodes, nodes_count);


    // long long total_count = compute_n_steps(nodes, nodes_count, steps, debug, log_file);
    // long long total_count = compute_n_nodes(nodes, nodes_count, steps, debug, log_file);
    // if (debug)
    //     print_nodes_array(nodes, total_count);

    // multithreaded version ------------------------------------
    long long total_count = 0;

    pthread_t tid;
    thread_args args;

    // for(int i=0; i<8; i++)
    // {
        args.input_list_id = list_count;
        args.output_list_id_1 = list_count + 1;
        args.steps = steps;
        args.debug = debug;
        pthread_create(&tid, NULL, compute_n_steps_thread, &args);
        pthread_join(tid, NULL);
        printf("tid result: %lld\n", args.nodes_count);

        total_count += args.nodes_count;
    // }
    // ----------------------------------------------------------

    time(&end_t);
    diff_t = difftime(end_t, start_t);

    // results output ------------------------------------------------------------------
    printf("[main] nodes count after %d steps: %lld\n", steps, total_count);
    printf("[main] execution time = %f s (%s)\n\n", diff_t, get_time_string(diff_t));

    fprintf(log_file, "\n[finished %d steps] nodes count: %lld\n\n", steps, total_count);
    fprintf(log_file, "Execution time = %f s (%s)\n", diff_t, get_time_string(diff_t));
    // results output ------------------------------------------------------------------

    // close used files
    fclose(log_file);

    // free used memory
    free(nodes);

    return 0;
}
