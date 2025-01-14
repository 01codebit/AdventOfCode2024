#include "computing.h"

expansion expand_disk_map(disk_map m)
{
    int chunks = 1;
    // printf("[expand_disk_map] Try to allocate %zu bytes for locations array\n", CHUNK_SIZE);

    int chunks_fids = 1;
    ULLONG *file_ids = (ULLONG *)malloc(CHUNK_SIZE * chunks_fids * sizeof(ULLONG));
    if (file_ids == NULL)
    {
        printf("[expand_disk_map] Cannot allocate %zu bytes for locations array\n", CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    int file_or_space = 0;
    ULLONG file_id = 0;

    int len = 0;
    printf("[expand_disk_map] scan %d values\n", m.length);
    for (int i = 0; i < m.length; i++)
    {
        int current = m.map[i];

        if ((len + current) > (CHUNK_SIZE * chunks_fids))
        {
            chunks_fids++;
            file_ids = (ULLONG *)realloc(file_ids, chunks_fids * CHUNK_SIZE * sizeof(ULLONG));
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
            // printf("set -1 for %d locations starting from %d\n", current, len);
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
    int i = e.length - 1;
    for (i = e.length - 1; i > 0; i--)
    {
        if (e.locations[i] > -1)
            break;
    }
    return i;
}

ULLONG arrange_expansion(expansion ex)
{
    ULLONG checksum = 0;
    // int free_index = first_free_index(ex);
    // int last_index = last_used_index(ex);

    // while (free_index < last_index)
    // {
    //     ex.locations[free_index] = ex.locations[last_index];
    //     ex.locations[last_index] = -1;

    //     free_index = first_free_index(ex);
    //     last_index = last_used_index(ex);
    // }

    for(int i=0; i<ex.length; i++)
    {
        if(ex.locations[i] < 0)
        {
            int last_index = last_used_index(ex);
            if(last_index>i)
            {
                ex.locations[i] = ex.locations[last_index];
                ex.locations[last_index] = -1;
            }
            else break;
        }
        checksum += ex.locations[i] * i;
    }

    return checksum;
}

ULLONG compute_checksum(expansion ex)
{
    ULLONG res = 0;

    ULLONG free_index = first_free_index(ex);
    ULLONG last_index = last_used_index(ex);

    FILE *output = fopen("compute_checksum_log.txt", "w");

    ULLONG max = 0;
    for (ULLONG i = 0; i < free_index; i++)
    {
        if (ex.locations[i] > max)
            max = ex.locations[i];
        // if (ex.locations[i] == ex.max_file_id)
        fprintf(output, "[%5d/%d] %lld += %lld * %lld (= %lld)\n", i, free_index, res, ex.locations[i], i, ex.locations[i] * i);

        if (ex.locations[i] < 0)
            printf("[compute_checksum] ERROR in %lld value: %lld\n", i, ex.locations[i]);

        res += ex.locations[i] * i;
    }

    if (max != ex.max_file_id)
        printf("[compute_checksum] ERROR found max file id %lld: must be %lld\n", max, ex.max_file_id);

    fprintf(output, "\nchecksum: %lld\n", res);
    fclose(output);

    return res;
}