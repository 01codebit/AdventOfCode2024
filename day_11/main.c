#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../common/printer.h"
#include "file_reader.h"
#include "computing.h"

void print_linked_list(linked_list **plist)
{
    linked_list *list = *plist;
    node *current_node = list->first;
    printf("[print_linked_list] linked list (len: %d): ", list->count);

    int count = 0;
    while (current_node != NULL)
    {
        printf("[%d] %lld ", count, current_node->value);
        current_node = current_node->next;
        count++;
        if (current_node != NULL)
            printf("-> ");
    }
    printf(".\n");
}

linked_list *read_linked_list(char *filename, int debug)
{
    char *list_str = read_line(filename, debug);
    // printf("[read_linked_list] read linked list string: %s\n", list_str);

    linked_list *llist = convert_to_linked_list(list_str, debug);

    return llist;
}

void split_linked_list(const char *filename_format, int list_number, int new_list_number, int debug)
{
    char llist_filename[30];
    sprintf(llist_filename, filename_format, list_number);
    linked_list *ll = read_linked_list(llist_filename, debug);
    if (debug)
        print_linked_list(&ll);

    long long middle_index = (ll->count / 2) + 1;

    node *middle = ll->first;
    for (long long i = 0; i < middle_index - 1; i++)
    {
        middle = middle->next;
    }
    node *last = middle;
    middle = middle->next;
    last->next = NULL;

    ll->count = middle_index;

    if (debug)
        print_linked_list(&ll);

    linked_list *new_llist = (linked_list *)malloc(sizeof(linked_list));
    new_llist->first = middle;
    new_llist->count = 0;
    node *x = new_llist->first;
    while (x != NULL)
    {
        new_llist->count++;
        x = x->next;
    }

    if (debug)
        printf("[split_linked_list] new list: ");
    if (debug)
        print_linked_list(&new_llist);

    if (debug)
        printf("[split_linked_list] write LEFT list #%lld file '%s'\n", list_number, llist_filename);
    if (debug)
        print_linked_list(&ll);
    print_list_to_file(llist_filename, &ll);

    sprintf(llist_filename, filename_format, new_list_number);
    if (debug)
        printf("[split_linked_list] write RIGHT list #%lld file '%s'\n", new_list_number, llist_filename);
    if (debug)
        print_linked_list(&new_llist);
    print_list_to_file(llist_filename, &new_llist);

    if (debug)
        printf("--------------------------------------------------\n");

    free(ll);
    free(new_llist);
}

int main(int argc, char *argv[])
{

    size_t s = sizeof(long long int);
    printf("sizeof(long long int): %lld\n", s); return 0;

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
    printf("[main] steps: %d, debug: %d\n", steps, debug);
    printf("[main] read file: '%s'\n", filename);

    // first pass ------------------------------------------------
    linked_list *stones = read_linked_list(filename, debug);
    print_linked_list(&stones);

    int list_lengths[MAX_LISTS];
    long long total_nodes = stones->count;

    const char *filename_format = "output/linked_list_%d.txt";
    int llist_count = 1;
    char llist_filename[30];
    sprintf(llist_filename, filename_format, llist_count - 1);
    print_list_to_file(llist_filename, &stones);
    // first pass [end] -----------------------------------------

    // second pass -----------------------------------------------
    printf("\n");

    long long max_list_len = 0;

    FILE *log_file = fopen("log.txt", "w");
    if (!log_file)
        printf("ERROR opening log file!\n");

    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    fprintf(log_file, "Current local time and date: %s\n", asctime(info));
    fprintf(log_file, "Limits: max lists: %lld, max nodes: %lld\n", MAX_LISTS, MAX_LISTS * MAX_NODES);
    fprintf(log_file, "Steps: %d\n\n", steps);

    fprintf(log_file, "Step\tLists\tNodes\n-----------------------------------\n");

    printf("Limits: max lists: %lld, max nodes: %lld\n", MAX_LISTS, MAX_LISTS * MAX_NODES);
    printf("Steps: %d\n\n", steps);

    time(&start_t);

    time_t step_start_t, step_end_t;
    double step_diff_t;

    for (int i = 0; i < steps; i++)
    {
        time(&step_start_t);

        total_nodes = 0;

        if (debug)
            printf("[main][step %02d/%02d] lists count: %d\n", i + 1, steps, llist_count);

        // fprintf(log_file, "[main][step %02d/%02d] lists count: %d\n", i + 1, steps, llist_count);

        if (llist_count > MAX_LISTS || total_nodes > MAX_NODES * MAX_LISTS)
        {
            printf("ERROR: too many lists\n");
            fprintf(log_file, "\nEXIT FAILURE: too many lists or too many nodes.\n");
            time(&end_t);
            diff_t = difftime(end_t, start_t);
            fprintf(log_file, "Execution time = %f s\n", diff_t);

            fclose(log_file);
            return EXIT_FAILURE;
        }

        for (int f = 0; f < llist_count; f++)
        {
            sprintf(llist_filename, filename_format, f);
            linked_list *llist = read_linked_list(llist_filename, debug);

            blink_step(&llist, debug);
            total_nodes += llist->count;
            list_lengths[f] = llist->count;

            if (max_list_len < llist->count)
                max_list_len = llist->count;

            if (debug)
                print_linked_list(&llist);
            if (debug)
                printf("[main] write list #%lld file '%s'\n", f, llist_filename);
            print_list_to_file(llist_filename, &llist);

            free(llist);
        }

        if (debug)
        {
            printf("\npress any key to continue...");
            while (!getchar())
            {
            }
        }

        if (max_list_len > MAX_NODES)
        {
            // printf("[main] max_list_len is %lld (> %lld) ===SPLIT THE LIST===\n", max_list_len, MAX_NODES);

            int old_llist_count = llist_count;
            for (int f = 0; f < old_llist_count; f++)
            {
                if (list_lengths[f] > MAX_NODES)
                {
                    if (debug)
                        printf("[main] list_len of list #%d is %lld (> %lld) ===SPLIT THE LIST===\n", f, list_lengths[f], MAX_NODES);

                    split_linked_list(filename_format, f, llist_count, debug);
                    llist_count++;
                }
            }
        }
        else if (debug)
            printf("[main] no split needed\n");

        time(&step_end_t);
        step_diff_t = difftime(step_end_t, step_start_t);
        fprintf(log_file, "[%02d/%02d] %5d %25lld (%f s)\n", i + 1, steps, llist_count, total_nodes, step_diff_t);

        // if (debug)
            printf("[main][%02d/%02d] %5d %25lld (%f s)\n", i + 1, steps, llist_count, total_nodes, step_diff_t);

        if (debug)
        {
            printf("\npress any key to continue...");
            while (!getchar())
            {
            }
        }
    }
    // second pass [end] -----------------------------------------

    time(&end_t);
    diff_t = difftime(end_t, start_t);

    printf("\n[main][finished %d steps] TOTAL NODES: %lld\n\n", steps, total_nodes);
    printf("Execution time = %f s\n", diff_t);

    fprintf(log_file, "\n[finished %d steps] TOTAL NODES: %lld\n\n", steps, total_nodes);
    fprintf(log_file, "Execution time = %f s\n", diff_t);

    printf("\n");

    // free used memory
    fclose(log_file);

    return 0;
}
