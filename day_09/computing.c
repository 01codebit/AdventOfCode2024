#include "computing.h"

expansion expand_disk_map(disk_map m)
{
    int chunks = 1;
    printf("[expand_disk_map] Try to allocate %zu bytes for string\n", CHUNK_SIZE);

    int chunks_fids = 1;
    long long int *file_ids = (long long int *)malloc(CHUNK_SIZE * chunks_fids * sizeof(long long int));
    if (file_ids == NULL)
    {
        printf("[expand_disk_map] Cannot allocate %zu bytes for string\n", CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    int file_or_space = 0;
    long long int file_id = 0;

    int len = 0;

    for (int i = 0; i < m.length; i++)
    {
        int current = m.map[i];

        if ((len + current) > (CHUNK_SIZE * chunks_fids))
        {
            chunks_fids++;
            file_ids = (long long int *)realloc(file_ids, chunks_fids * CHUNK_SIZE * sizeof(long long int));
            // printf("[expand_disk_map] Try to reallocate %zu bytes for file_ids\n", CHUNK_SIZE * chunks_fids);
            if (file_ids == NULL)
            {
                printf("[expand_disk_map] Cannot reallocate %zu bytes for file_ids\n", chunks_fids * CHUNK_SIZE);
                exit(EXIT_FAILURE);
            }
            // else
            //     printf("[expand_disk_map] New file ids size: %zu\n", chunks_fids * CHUNK_SIZE);
        }

        if (file_or_space == 0)
        {
            for (int k = 0; k < current; k++)
            {
                file_ids[len + k] = file_id;
            }
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

    printf("[expand_disk_map] result length: %u, max file_id: %lld\n", len, file_id-1);

    expansion ex;
    ex.locations = file_ids;
    ex.length = len;

    return ex;
}


int first_free_index(expansion e)
{
    int i = 0;
    for (i = 0; i < e.length; i++)
    {
        if (e.locations[i] == -1)
            break;
    }
    return i;
}

int last_used_index(expansion e)
{
    int i = e.length-1;
    for (i = e.length-1; i > 0; i--)
    {
        if (e.locations[i] > -1)
            break;
    }
    return i;
}

void arrange_expansion(expansion ex)
{
    int free_index = first_free_index(ex);
    int last_index = last_used_index(ex);

    while (free_index < last_index)
    {
        ex.locations[free_index] = ex.locations[last_index];
        ex.locations[last_index] = -1;

        free_index = first_free_index(ex);
        last_index = last_used_index(ex);

        // printf("ex: %s\n", ex);
    }
}

long long int compute_checksum(expansion ex)
{
    long long int res = 0L;

    int free_index = first_free_index(ex);

    for (int i = 0L; i < free_index; i++)
    {        
        res += ex.locations[i] * (long long int)i;
    }

    return res;
}