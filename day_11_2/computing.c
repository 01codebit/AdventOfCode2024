#include "computing.h"

char *get_time_string(float time)
{
    char *result = (char *)malloc(20 * sizeof(char));
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
        if (i > 30)
            break;
    }
    printf("\n");
}

long long convert_to_node_array(node *nodes, char *str, long long chunks, int debug)
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

        if (count >= chunks * CHUNK_SIZE)
        {
            chunks++;
            nodes = (node *)realloc(nodes, chunks * CHUNK_SIZE * sizeof(node));
        }

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

void compute_n_steps_thread(thread_args *args)
{
    pthread_t tid = pthread_self();
    printf("[compute_n_steps_thread] Thread #%lld\n", tid);

    // for(int i=0; i<args->start_node_count; i++)
    // {
    //     printf("[compute_n_steps_thread] Thread #%lld node[%d]: %lld\n", tid, i, (args->nodes)[i]->value);
    // }

    // long long result = compute_n_steps(*(args->nodes), args->start_node_count, args->steps, args->debug, args->log_file);
    // args->result = result;

    const char *filename_format = "output/list_%lld.txt";
    char input_filename[30];
    sprintf(input_filename, filename_format, args->input_list_id);
    char *list = read_line(input_filename, 0);

    long long chunks = 1;
    node *nodes = (node *)malloc(chunks * CHUNK_SIZE * sizeof(node));

    long long nodes_count = convert_to_node_array(nodes, list, chunks, 0);

    long long count = compute_n_steps(nodes, nodes_count, args->steps, args->debug, NULL);
    printf("[compute_n_steps_thread][thread#%lld] count: %lld\n", tid, count);

    char output_filename[30];
    sprintf(output_filename, filename_format, args->output_list_id_1);
    printf("[compute_n_steps_thread][thread#%lld] try to write file '%s'\n", tid, output_filename);
    print_list_to_file(output_filename, &nodes, count);

    args->nodes_count = count;
}

long long compute_n_steps(node *nodes, long long start_node_count, int steps, int debug, FILE *log_file)
{
    pthread_t tid = pthread_self();
    printf("[compute_n_steps] Thread #%lld\n", tid);
    if (log_file)
        fprintf(log_file, "[compute_n_steps:thread#%lld] count: %lld, steps: %d\n", tid, start_node_count, steps);

    printf("[compute_n_steps:thread#%lld] count: %lld, steps: %d\n", tid, start_node_count, steps);

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
    long long chunks = start_node_count < CHUNK_SIZE ? 1 : (int)(start_node_count / CHUNK_SIZE + 1);
    // printf("[compute_n_steps] start_node_count: %lld  CHUNK_SIZE: %d chunks: %lld (%lld)\n", start_node_count, CHUNK_SIZE, chunks, chunks * CHUNK_SIZE);
    node *buffer = (node *)malloc(chunks * CHUNK_SIZE * sizeof(node));
    if (buffer == NULL)
    {
        printf("ERROR malloc buffer\n");
        return 0;
    }
    for (long long i = 0; i < start_node_count; i++)
    {
        // inverted
        // printf("buffer[%lld] = nodes[%ld] = [%d %lld]\n", start_node_count - 1 - i, i, nodes[i].step, nodes[i].value);
        buffer[start_node_count - 1 - i] = nodes[i];
    }
    if (debug)
        printf("[compute_n_steps] inverted buffer array:\n");
    if (debug)
        print_nodes_array(buffer, start_node_count);

    printf("[compute_n_steps] total_node_count:\n");

    // if (log_file)
    //     fprintf(log_file, "Last node of first level | total nodes start -> end\n", zero_level_nodes, start_node_count, total_node_count);

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
                    fprintf(log_file, "[compute_n_steps:thread#%lld] %25lld (time: %s)\n", tid, total_node_count, diff_s);
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
                // printf("[compute_n_steps] ****** nodes array reallocated to size %lld\n", nodes_chunks * CHUNK_SIZE);
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
                if (total_node_count >= nodes_chunks * CHUNK_SIZE)
                {
                    nodes_chunks++;
                    nodes = (node *)realloc(nodes, nodes_chunks * CHUNK_SIZE * sizeof(node));
                }
                // printf("nodes[%lld/%lld] = %lld\n", total_node_count, nodes_chunks * CHUNK_SIZE, nd.value);
                nodes[total_node_count] = nd;

                total_node_count++;
                if (debug)
                    printf("\n");

                if (OUTPUT)
                    fprintf(output, "%lld ", nd.value);
            }
        }
    }

    
    fprintf(log_file, "[compute_n_steps] nodes array reallocated to size %lld\n", nodes_chunks * CHUNK_SIZE);

    time(&end_t);
    diff_t = difftime(end_t, start_t);
    char *diff_s = get_time_string(diff_t);
    printf(" %25lld (time: %s)\n", total_node_count, diff_s);
    if (log_file)
        fprintf(log_file, "[compute_n_steps:thread#%lld] %25lld (time: %s)\n", tid, total_node_count, diff_s);

    // printf("buffer chunks used: %d\n", chunks);
    printf("\n");

    if (OUTPUT)
    {
        fprintf(output, "\n");
        fclose(output);
    }

    free(buffer);

    if (debug)
        printf("\n");

    return total_node_count;
}

long long compute_n_nodes(node *nodes, long long start_node_count, int steps, int debug, FILE *log_file)
{
    long long total_node_count = 0;
    long long current_step_nodes_count = start_node_count;
    printf("[compute_n_nodes] start_node_count: %lld, steps: %d\n", start_node_count, steps);
    long long buffer_size = start_node_count + 1;
    node *buffer = (node *)malloc(buffer_size * sizeof(node));
    if (buffer == NULL)
    {
        printf("ERROR malloc buffer\n");
        return 0;
    }
    memset(buffer, 0, start_node_count);

    for (int i = 1; i <= steps; i++)
    {
        total_node_count = 0;
        printf("[compute_n_nodes] step %d/%d nodes: %lld ", i, steps, current_step_nodes_count);
        for (long long n = 0; n < current_step_nodes_count; n++)
        {
            node nd = nodes[n];

            // printf("[compute_n_nodes]   node %lld/%lld value: %lld\n", n+1, current_step_nodes_count, nd.value);

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
                // printf("[compute_n_nodes]     buffer[%lld] = %lld\n", total_node_count, nd.value);
                buffer[total_node_count] = nd;
                total_node_count++;
            }
            else if (len > 1 && len % 2 == 0)
            {
                long long x = nd.value;
                int len_h = len / 2;
                long long p = pow(10, len_h);
                long long x1 = x / p;
                long long r = x1 * p;
                long long x2 = x - r;

                nd.value = x1;
                // printf("[compute_n_nodes]     buffer[%lld] = %lld\n", total_node_count, nd.value);
                buffer[total_node_count] = nd;
                total_node_count++;

                node new_node;
                new_node.step = nd.step;
                new_node.value = x2;

                // printf("[compute_n_nodes]     buffer[%lld] = %lld\n", total_node_count, new_node.value);
                buffer[total_node_count] = new_node;
                total_node_count++;
            }
            else
            {
                nd.value *= 2024;
                // printf("[compute_n_nodes]     buffer[%lld] = %lld\n", total_node_count, nd.value);
                buffer[total_node_count] = nd;
                total_node_count++;
            }

            if ((total_node_count + 3) >= buffer_size)
            {
                // printf("[compute_n_nodes] realloc buffer total_node_count: %lld > buffer_size: %lld ---> new size: %lld\n",
                //    total_node_count, buffer_size, buffer_size + CHUNK_SIZE);
                buffer_size += CHUNK_SIZE;
                buffer = (node *)realloc(buffer, buffer_size * sizeof(node));
                if (buffer == NULL)
                {
                    printf("ERROR reallocating buffer\n");
                }
            }
        }

        nodes = (node *)realloc(nodes, total_node_count * sizeof(node));
        // memcpy(nodes, buffer, total_node_count);
        for (long long i = 0; i < total_node_count; i++)
        {
            nodes[i] = buffer[i];
        }

        current_step_nodes_count = total_node_count;
        printf(" ---> %lld\n", current_step_nodes_count);
    }

    if (debug)
    {
        printf("[compute_n_nodes] total_node_count: %lld - nodes array:\n", total_node_count);
        print_nodes_array(nodes, total_node_count);
    }

    // if (OUTPUT)
    {
        char output_filename[30];
        sprintf(output_filename, "output/nodes_%lld_steps_%d.txt", total_node_count, steps);
        FILE *output = fopen(output_filename, "w");
        if (output)
        {
            for (long long i = 0; i < total_node_count; i++)
            {
                fprintf(output, "%lld ", nodes[i].value);
            }

            fprintf(output, "%n");
            fclose(output);
        }
        else
        {
            printf("[compute_n_steps] ERROR opening output file.");
        }
    }

    free(buffer);

    return total_node_count;
}

long long compute_step_to_file(const char *filename_format, int source_file_counter)
{
    char source_filename[30];
    sprintf(source_filename, filename_format, source_file_counter);

    char dest_filename[30];
    int dest_file_counter = source_file_counter + 1;
    sprintf(dest_filename, filename_format, dest_file_counter);

    // printf("[compute_step_to_file] open source file '%s'\n", source_filename);
    FILE *source = fopen(source_filename, "r");
    if (!source)
    {
        fprintf(stderr, "[compute_step_to_file] unable to open the file '%s': %s [errno:%d]\n", source_filename, strerror(errno), errno);
        return;
    }

    // printf("[compute_step_to_file] open dest file '%s'\n", dest_filename);
    FILE *dest = fopen(dest_filename, "w");
    if (!dest)
    {
        fprintf(stderr, "[compute_step_to_file] unable to open the file '%s': %s [errno:%d]\n", dest_filename, strerror(errno), errno);
        return;
    }

    char buffer[30];
    int count = 0;
    char ch = 0;
    char out_buffer[30];

    long long int node_count = 0;

    while (ch != EOF)
    {
        ch = fgetc(source);
        if (ch == ' ' || ch == EOF)
        {
            buffer[count] = '\0';
            // printf("%s (len: %d)\n", buffer, strlen(buffer));
            count = 0;

            char *end_ptr;
            long long val = strtoll(buffer, &end_ptr, 10);

            /*
                first applicable rule in this list:

                1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
                2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
                3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
            */

            if (val == 0)
            {
                val = 1;
                sprintf(out_buffer, "%lld", val);
                
                node_count++;
            }
            else if (strlen(buffer) % 2 == 0)
            {
                int half = strlen(buffer) / 2;
                char *end_ptr;

                char t1[30];
                for(int i = 0; i < half; i++)
                {
                    t1[i] = buffer[i];
                }
                t1[half] = '\0';
                long long int v1 = strtoll(t1, &end_ptr, 10);

                char t2[30];
                for(int i = half; i < strlen(buffer); i++)
                {
                    t2[i-half] = buffer[i];
                }
                t2[half] = '\0';
                long long int v2 = strtoll(t2, &end_ptr, 10);

                sprintf(out_buffer, "%lld %lld", v1, v2);
                // printf("'%s' len: %d half: %d ---> t1:'%s' t2:'%s'\n", buffer, strlen(buffer), half, t1, t2);

                node_count += 2;
            }
            else
            {
                val *= 2024;
                sprintf(out_buffer, "%lld", val);
                
                node_count++;
            }

            fprintf(dest, "%s", out_buffer);

            if(ch!=EOF) fprintf(dest, " ");
        }
        else
        {
            buffer[count] = ch;
            count++;
        }
    }

    int fc = fclose(source);
    if(fc!=0)
    {
        fprintf(stderr, "[compute_step_to_file] error closing source: %s [errno:%d]\n", strerror(errno), errno);
    }
    fc = fclose(dest);
    if(fc!=0)
    {
        fprintf(stderr, "[compute_step_to_file] error closing dest: %s [errno:%d]\n", strerror(errno), errno);
    }

    return node_count;
}