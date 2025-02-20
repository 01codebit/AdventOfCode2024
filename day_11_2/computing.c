#include "computing.h"
#include <string.h>
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    long long int id; /* we'll use this field as the key */
    long long int result;
    UT_hash_handle hh; /* makes this structure hashable */
} entry;

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

long long int convert_to_values_array(long long int *values, char *str, long long chunks, int debug)
{
    char *token = strtok(str, " ");
    char *end_ptr;

    long long count = 0;

    while (token)
    {
        long long value = strtoll(token, &end_ptr, 10);

        if (count >= chunks * CHUNK_SIZE)
        {
            chunks++;
            values = (long long int *)realloc(values, chunks * CHUNK_SIZE * sizeof(long long int));
        }

        values[count] = value;

        count++;

        token = strtok(NULL, " ");
    }

    return count;
}

entry *entry_cache = NULL;

void cache_insert(long long int value, int step, long long int result)
{
    entry *e = (entry *)malloc(sizeof(entry));
    long long int id = value * 100 + step;
    e->id = id;
    e->result = result;

    HASH_ADD_INT(entry_cache, id, e);
}

long long int cache_lookup(long long int value, int step)
{
    long long int id = value * 100 + step;
    entry *e = NULL;
    HASH_FIND_INT(entry_cache, &id, e);

    if (e)
        return e->result;
    else
        return -1;
}

long long int compute_node_r(long long int value, int step, int target)
{
    if (step == target)
        return 1;

    // lookup cache
    long long int cached = cache_lookup(value, step);
    if (cached > 0)
        return cached;

    // compute
    int len = (int)floor(log10(value) + 1);
    int new_step = step + 1;

    /*
        first applicable rule in this list:

        1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
        2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
        3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
    */

    long long int result = 0;

    if (value == 0)
    {
        result = compute_node_r(value + 1, new_step, target);
    }
    else if (len > 1 && len % 2 == 0)
    {
        int len_h = len / 2;
        long long p = pow(10, len_h);
        long long x1 = value / p;
        long long r = x1 * p;
        long long x2 = value - r;

        result = compute_node_r(x1, new_step, target) + compute_node_r(x2, new_step, target);
    }
    else
    {
        result = compute_node_r(value * 2024, new_step, target);
    }

    // insert in cache
    cache_insert(value, step, result);

    return result;
}

long long int compute_depth_r(long long int *values, int values_count, int target)
{
    long long int result = 0;

    for (int i = 0; i < values_count; i++)
    {
        result += compute_node_r(values[i], 0, target);
    }

    return result;
}