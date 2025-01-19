#include "computing.h"

expansion expand_disk_map(const disk_map m)
{
    int chunks = 1;

    int chunks_fids = 1;
    LLONG *file_ids = (LLONG *)malloc(CHUNK_SIZE * chunks_fids * sizeof(LLONG));
    if (file_ids == NULL)
    {
        printf("[expand_disk_map] Cannot allocate %zu bytes for locations array\n", CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    int chunks_sizes = 0;
    int *file_sizes = (int *)malloc(CHUNK_SIZE * chunks_sizes * sizeof(int));
    if (file_sizes == NULL)
    {
        printf("[expand_disk_map] Cannot allocate %zu bytes for file_sizes array\n", CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    int file_or_space = 0;
    int file_id = 0;

    int len = 0;
    printf("[expand_disk_map] scan %d values\n", m.length);
    for (int i = 0; i < m.length; i++)
    {
        int current = m.map[i];

        if ((len + current) > (CHUNK_SIZE * chunks_fids))
        {
            chunks_fids++;
            file_ids = (LLONG *)realloc(file_ids, chunks_fids * CHUNK_SIZE * sizeof(LLONG));
            if (file_ids == NULL)
            {
                printf("[expand_disk_map] Cannot reallocate %zu bytes for file_ids\n", chunks_fids * CHUNK_SIZE);
                exit(EXIT_FAILURE);
            }
        }

        if (file_or_space == 0)
        {
            for (int k = 0; k < current; k++)
            {
                file_ids[len + k] = file_id;
            }

            if (file_id > chunks_sizes * CHUNK_SIZE)
            {
                chunks_sizes++;
                file_sizes = (int *)realloc(file_sizes, chunks_sizes * CHUNK_SIZE * sizeof(int));
                if (file_sizes == NULL)
                {
                    printf("[expand_disk_map] Cannot reallocate %zu bytes for file_sizes\n", chunks_sizes * CHUNK_SIZE);
                    exit(EXIT_FAILURE);
                }
            }
            file_sizes[file_id] = current;

            file_id++;
            file_or_space = 1;
        }
        else if (file_or_space == 1)
        {
            for (int k = 0; k < current; k++)
            {
                file_ids[len + k] = -1;
            }
            file_or_space = 0;
        }

        len += current;
    }

    printf("[expand_disk_map] result length: %d, max file_id: %lld\n", len, file_id - 1);

    expansion ex;
    ex.locations = file_ids;
    ex.length = len;
    ex.max_file_id = file_id - 1;

    return ex;
}

int first_free_index(const expansion e)
{
    int i = 0;
    for (i = 0; i < e.length; i++)
    {
        if (e.locations[i] == -1)
            break;
    }
    return i;
}

int last_used_index(const expansion e)
{
    int i = e.length - 1;
    for (i = e.length - 1; i > 0; i--)
    {
        if (e.locations[i] > -1)
            break;
    }
    return i;
}

LLONG arrange_expansion(const expansion ex)
{
    LLONG checksum = 0;

    for (int i = 0; i < ex.length; i++)
    {
        if (ex.locations[i] < 0)
        {
            int last_index = last_used_index(ex);
            if (last_index > i)
            {
                ex.locations[i] = ex.locations[last_index];
                ex.locations[last_index] = -1;
            }
            else
                break;
        }
        checksum += ex.locations[i] * i;
    }

    return checksum;
}

LLONG compute_checksum(const expansion ex)
{
    LLONG checksum = 0;

    FILE *output = fopen("compute_checksum_log.txt", "w");

    for (int i = 0; i < ex.length; i++)
    {
        LLONG current = ex.locations[i];

        if (current > ex.max_file_id)
        {
            printf("[compute_checksum] ERROR found file id %d (max file id is %d)\n", current, ex.max_file_id);
        }
        else if (current > 0)
        {
            LLONG prev = checksum;
            checksum += current * i;

            fprintf(output, "[%5d/%d] %lld += %lld * %d (= %lld) = %lld \n", i, ex.length, prev, current, i, current * i, checksum);
        }
        else
        {
            fprintf(output, "[%5d/%d] %lld (%lld)\n", i, ex.length, checksum, current);
        }
    }

    fprintf(output, "\nchecksum: %lld\n", checksum);
    fclose(output);

    return checksum;
}

int first_free_n_index(const expansion e, int n)
{
    int start = 0;
    int count = 0;

    for (int i = 0; i < e.length; i++)
    {
        if (e.locations[i] == -1)
        {
            if (count == 0)
            {
                start = i;
            }
            count++;

            if (count == n)
            {
                break;
            }
        }
        else
        {
            count = 0;
        }
    }

    return start;
}

n_index last_used_n_index(const expansion e, int prev_id)
{
    n_index ind;

    int count = 0;
    int start = -1;
    int current_file_id = -1;

    for (int i = e.length - 1; i > 0; i--)
    {
        if (e.locations[i] > -1 && current_file_id == -1 && e.locations[i] < prev_id)
        {
            current_file_id = e.locations[i];
            count++;
            start = i;
        }
        else if (e.locations[i] > -1 && e.locations[i] == current_file_id)
        {
            count++;
            start = i;
        }
        else if (e.locations[i] == -1 && current_file_id != -1)
        {
            break;
        }
    }

    ind.file_id = current_file_id;
    ind.start = start;
    ind.count = count;

    if (count > 9)
        printf("ERROR: file_id: %d has %d locations\n", current_file_id, count);

    return ind;
}

void arrange_expansion_n(const expansion ex)
{
    n_index last_index = last_used_n_index(ex, ex.max_file_id + 1);
    int free_index = first_free_n_index(ex, last_index.count);

    while (last_index.start >= 0)
    {
        if ((free_index + last_index.count-1) < last_index.start)
        {
            for (int i = 0; i < last_index.count; i++)
            {
                ex.locations[free_index + i] = ex.locations[last_index.start + i];
                ex.locations[last_index.start + i] = -1;
            }
        }

        last_index = last_used_n_index(ex, last_index.file_id);
        free_index = first_free_n_index(ex, last_index.count);
    }
}
