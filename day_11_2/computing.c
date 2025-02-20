#include "computing.h"
#include <string.h>
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct
{
    LLINT id; /* we'll use this field as the key */
    LLINT result;
    UT_hash_handle hh; /* makes this structure hashable */
} entry;


LLINT convert_to_values_array(LLINT *values, char *str, LLINT chunks, int debug)
{
    char *token = strtok(str, " ");
    char *end_ptr;

    LLINT count = 0;

    while (token)
    {
        LLINT value = strtoll(token, &end_ptr, 10);

        if (count >= chunks * CHUNK_SIZE)
        {
            chunks++;
            values = (LLINT *)realloc(values, chunks * CHUNK_SIZE * sizeof(LLINT));
        }

        values[count] = value;

        count++;

        token = strtok(NULL, " ");
    }

    return count;
}



entry *entry_cache = NULL;
int total_cache_size = 0;
LLINT cache_hits = 0;

#define POOL_SIZE 200000
entry *entry_pool[POOL_SIZE];
int next_entry = 0;

void cache_pool_init()
{
    for(int i=0; i<POOL_SIZE; i++)
    {
        entry *e = (entry *)malloc(sizeof(entry));
        entry_pool[i] = e;
    }
}

void free_cache_pool()
{
    for(int i=0; i<POOL_SIZE; i++)
    {
        free(entry_pool[i]);
    }
}

entry *get_entry()
{
    entry *e = entry_pool[next_entry];
    next_entry++;
    return e;
}

// LLINT array_cache[40952650956874];

LLINT max_value_in_cache = 0;

LLINT get_id(LLINT value, int step)
{
    LLINT id = (value * 100 + step);
    return id;
}


void cache_insert(LLINT value, int step, LLINT result)
{
    entry *e = get_entry();
    LLINT id = get_id(value, step);
    e->id = id;
    e->result = result;

    HASH_ADD_INT(entry_cache, id, e);
    total_cache_size++;

    if(max_value_in_cache<id) max_value_in_cache = id;
}

LLINT cache_lookup(LLINT value, int step)
{
    LLINT id = get_id(value, step);
    entry *e = NULL;
    HASH_FIND_INT(entry_cache, &id, e);

    if (e)
    {
        cache_hits++;
        return e->result;
    }
    else
        return 0;
}

LLINT computed_nodes = 0;

LLINT compute_node_r(LLINT value, int step, int target, int use_cache)
{
    computed_nodes++;
    if (step == target)
        return 1;

    // lookup cache
    if (use_cache)
    {
        LLINT cached = cache_lookup(value, step);
        if (cached != 0)
        {
            return cached;
        }
    }

    // compute
    int len = (int)floor(log10(value) + 1);
    int new_step = step + 1;

    /*
        first applicable rule in this list:

        1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
        2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
        3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
    */

    LLINT result = 0;

    if (value == 0)
    {
        result = compute_node_r(value + 1, new_step, target, use_cache);
    }
    else if (len > 1 && len % 2 == 0)
    {
        int len_h = len / 2;
        LLINT p = pow(10, len_h);
        LLINT x1 = value / p;
        LLINT r = x1 * p;
        LLINT x2 = value - r;

        result = compute_node_r(x1, new_step, target, use_cache) + compute_node_r(x2, new_step, target, use_cache);
    }
    else
    {
        result = compute_node_r(value * 2024, new_step, target, use_cache);
    }

    // insert in cache
    if (use_cache)
    {
        cache_insert(value, step, result);
    }

    return result;
}

LLINT compute_depth_r(LLINT *values, int values_count, int target, int use_cache)
{
    LLINT result = 0;

    for (int i = 0; i < values_count; i++)
    {
        result += compute_node_r(values[i], 0, target, use_cache);
    }

    printf("[compute_depth_r] computed nodes:     %lld\n", computed_nodes);
    printf("[compute_depth_r] total cache size:   %d\n", total_cache_size);
    printf("[compute_depth_r] cache hits:         %lld\n", cache_hits);
    
    printf("[compute_depth_r] max_value_in_cache: %lld\n", max_value_in_cache);
    
    return result;
}