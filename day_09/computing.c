#include "computing.h"

char *expand_disk_map(disk_map m)
{
    // char *res = (char *)malloc(CHUNK_SIZE * sizeof(char));
    char *res = (char *)malloc(100000 * sizeof(char));
    strset(res, 0);
    // int chunks = 1;

    int file_or_space = 0;
    int file_id = 0;

    int len = 0;

    for (int i = 0; i < m.length; i++)
    {
        printf("i: %d\r", i);
        int current = m.map[i];

        if (file_or_space == 0)
        {
            for (int k = 0; k < current; k++)
            {
                res[len + k] = file_id + '0';
            }
            file_id++;
            file_or_space = 1;
        }
        else if (file_or_space == 1)
        {
            for (int k = 0; k < current; k++)
            {
                res[len + k] = '.';
            }
            file_or_space = 0;
        }

        len += current;
        // if (len > (CHUNK_SIZE * chunks))
        // {
        //     chunks++;
        //     res = (char *)realloc(res, chunks * CHUNK_SIZE * sizeof(char));
        // }
    }
    printf("\n");

    printf("[expand_disk_map] result length: %d, len: %d\n", strlen(res), len);
    res[len] = '\0';
    return res;
}

int first_free_index(char *str)
{
    int i = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
            break;
    }
    return i;
}

int last_used_index(char *str)
{
    int i = strlen(str);
    for (i = strlen(str); i > 0; i--)
    {
        if (str[i] != '.' && str[i] != '\0' && str[i] != '\n')
            break;
    }
    return i;
}

void arrange_expansion(char *ex)
{
    int free_index = first_free_index(ex);
    int last_index = last_used_index(ex);

    while (free_index < last_index)
    {
        ex[free_index] = ex[last_index];
        ex[last_index] = '.';

        free_index = first_free_index(ex);
        last_index = last_used_index(ex);

        // printf("ex: %s\n", ex);
    }
}

long int compute_checksum(char *ex)
{
    long int res = 0;

    int free_index = first_free_index(ex);

    for (int i = 0; i < free_index; i++)
    {
        int x = ex[i] - '0';
        res += x * i;
    }

    return res;
}