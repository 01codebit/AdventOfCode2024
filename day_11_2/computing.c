#include "computing.h"

char *get_time_string(float time)
{
    char *result = (char *)malloc(20*sizeof(char));
    int s = time == 0 ? 0 : ((int)time) % 60;
    int m = time == 0 ? 0 : ((int)time - s) / 60;

    int h = 0;
    if ((m / 60) > 0)
    {
        int h = m / 60;
        m = m % 60;
    }
    sprintf(result, "%02d:%02d:%02d", h, m, s);

    return result;
}

void print_nodes_array(node *nodes, long long count)
{
    printf("[print_array] nodes: ");
    for (long long i = 0; i < count; i++)
    {
        printf("%lld ", nodes[i].value);
    }
    printf("\n");
}

long long convert_to_node_array(node *nodes, char *str, int debug)
{
    char *token = strtok(str, " ");
    char *end_ptr;

    long long count = 0;

    while (token)
    {
        long long val = strtoll(token, &end_ptr, 10);
        node nd;
        nd.value = val;
        nd.step = 0;
        nodes[count] = nd;

        count++;

        token = strtok(NULL, " ");
    }

    return count;
}

char *insert_char_in_string(char *str, char ch, int position)
{
    size_t sz = strlen(str) + 1;

    char *result = (char *)malloc(sz * sizeof(char));
    int offset = 0;
    for (int i = 0; i < sz; i++)
    {
        if (i == position)
        {
            result[i] = ch;
            offset = -1;
        }
        else
        {
            result[i] = str[i + offset];
        }
    }
    result[sz] = '\0';

    return result;
}

node pop_first_node(node *nodes, long long *count)
{
    node nd = nodes[0];
    for (long long i = 1; i < *count; i++)
    {
        nodes[i - 1] = nodes[i];
    }
    (*count)--;
    return nd;
}

void push_first_node(node nd, node *nodes, long long *count)
{
    (*count)++;
    for (long long i = *count; i > 0; i--)
    {
        nodes[i] = nodes[i - 1];
    }
    nodes[0] = nd;
}

long long compute_n_steps(node *nodes, long long start_node_count, int steps, int debug, FILE *log_file)
{
    long long total_node_count = 0;
    long long buffer_count = start_node_count;
    long long zero_level_nodes = 0;

    time_t start_t, end_t;
    double diff_t;

    FILE *output;
    if (OUTPUT)
    {
        output = fopen("output/node_list.txt", "w");
        if (!output)
        {
            printf("[compute_n_steps] ERROR opening output file.");
            return 0;
        }
    }

    int nodes_chunks = 1;
    int chunks = 1;
    node *buffer = (node *)malloc(chunks * CHUNK_SIZE * sizeof(node));
    for (long long i = 0; i < start_node_count; i++)
    {
        // inverted
        buffer[start_node_count - 1 - i] = nodes[i];
    }
    if (debug)
        printf("[compute_n_steps] inverted buffer array:\n");
    if (debug)
        print_nodes_array(buffer, start_node_count);

    printf("[compute_n_steps] total_node_count:\n");

    if (log_file)
        fprintf(log_file, "Last node of first level | total nodes start -> end\n", zero_level_nodes, start_node_count, total_node_count);

    while (buffer_count > 0)
    {
        // node nd = pop_first_node(buffer, &buffer_count);
        buffer_count--;
        node nd = buffer[buffer_count];
        if (nd.step == 0)
        {
            if (zero_level_nodes > 0)
            {
                time(&end_t);
                diff_t = difftime(end_t, start_t);
                char *diff_s = get_time_string(diff_t);

                printf(" %25lld (time: %s)\n", total_node_count, diff_s);
                if (log_file)
                    fprintf(log_file, " %25lld (time: %s)\n", total_node_count, diff_s);
            }

            time(&start_t);

            zero_level_nodes++;
            printf("[%lld/%lld] %25lld ...", zero_level_nodes, start_node_count, total_node_count);
            if (log_file)
                fprintf(log_file, "[%lld/%lld] %25lld ...", zero_level_nodes, start_node_count, total_node_count);
        }

        if (debug)
            printf("[%d][%lld]", nd.step, nd.value);

        if (nd.step == steps)
        {
            if (debug)
                printf("-> [%d][%lld]\n", nd.step, nd.value);

            if (total_node_count >= nodes_chunks * CHUNK_SIZE)
            {
                nodes_chunks++;
                nodes = (node *)realloc(nodes, nodes_chunks * CHUNK_SIZE * sizeof(node));
            }
            nodes[total_node_count] = nd;

            total_node_count++;
            if (OUTPUT)
                fprintf(output, "%lld ", nd.value);
        }

        while (nd.step < steps)
        {
            // char *end_ptr;
            // char val_str[50];
            // sprintf(val_str, "%lld", nd.value);
            // int len = strlen(val_str);

            int len = (int)floor(log10(nd.value) + 1);

            nd.step++;

            /*
                first applicable rule in this list:

                1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
                2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
                3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
            */

            if (nd.value == 0)
            {
                nd.value = 1;
            }
            else if (len > 1 && len % 2 == 0)
            {
                // int pos = len / 2;
                // strcpy(val_str, insert_char_in_string(val_str, ' ', pos));
                // char *left = strtok(val_str, " ");
                // char *right = strtok(NULL, " ");

                long long x = nd.value;
                int len_h = len / 2;
                long long p = pow(10, len_h);
                long long x1 = x / p;
                long long r = x1 * p;
                long long x2 = x - r;

                // nd.value = strtoll(left, &end_ptr, 10);
                nd.value = x1;

                node new_node;
                new_node.step = nd.step;
                // new_node.value = strtoll(right, &end_ptr, 10);
                new_node.value = x2;

                // push_first_node(new_node, buffer, &buffer_count);
                buffer[buffer_count] = new_node;
                buffer_count++;
                if (buffer_count >= chunks * CHUNK_SIZE)
                {
                    chunks++;
                    buffer = (node *)realloc(buffer, chunks * CHUNK_SIZE * sizeof(node));
                }
            }
            else
            {
                nd.value *= 2024;
            }

            if (debug)
                printf("-> [%d][%lld]", nd.step, nd.value);

            if (nd.step == steps)
            {
                // if (total_node_count >= nodes_chunks * CHUNK_SIZE)
                // {
                //     nodes_chunks++;
                //     nodes = (node *)realloc(nodes, nodes_chunks * CHUNK_SIZE * sizeof(node));
                // }
                // nodes[total_node_count] = nd;

                total_node_count++;
                if (debug)
                    printf("\n");

                if (OUTPUT)
                    fprintf(output, "%lld ", nd.value);
            }
        }
    }

    time(&end_t);
    diff_t = difftime(end_t, start_t);
    char* diff_s = get_time_string(diff_t);
    printf(" %25lld (time: %s)\n", total_node_count, diff_s);
    if (log_file)
        fprintf(log_file, " %25lld (time: %s)\n", total_node_count, diff_s);

    printf("buffer chunks used: %d\n", chunks);
    printf("\n");

    if (OUTPUT)
    {
        fprintf(output, "\n");
        fclose(output);
    }

    if (debug)
        printf("\n");
    return total_node_count;
}