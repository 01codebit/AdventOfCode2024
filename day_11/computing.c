#include "computing.h"

linked_list *convert_to_linked_list(const char *str, int debug)
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->count = 0;
    list->first = NULL;

    char str2[256];
    strcpy(str2, str);
    char *token = strtok(str2, " ");

    node *nd = (node *)malloc(sizeof(node));
    nd->value = token;

    list->first = nd;
    node *prev = nd;

    if (debug)
        printf("[convert_to_linked_list] add value: %s\n", nd->value);

    while (token)
    {
        if (list->count > 0)
        {
            node *nd = (node *)malloc(sizeof(node));
            nd->value = token;
            prev->next = nd;
            prev = nd;
            if (debug)
                printf("[convert_to_linked_list] add value: %s\n", nd->value);
        }

        list->count++;

        token = strtok(NULL, " ");
    }
    prev->next = NULL;

    return list;
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

char *blink_step_str(const char *stones, int debug)
{
    int chunks = 1;
    char *result = (char *)malloc(strlen(stones) * sizeof(char));
    char *working = (char *)malloc(strlen(stones) * sizeof(char));

    char app[50];

    strset(result, 0);
    strcpy(working, stones);

    int count = 0;

    char *token = strtok(working, " ");
    while (token)
    {
        count++;
        if (debug)
            printf("token: %s\n", token);

        /*
            first applicable rule in this list:

            1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
            2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
            3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.

        */

        if (strcmp(token, "0") == 0)
        {
            if (debug)
                printf("  apply rule #1");
            strcpy(app, "1");
        }
        else if (strlen(token) % 2 == 0)
        {
            if (debug)
                printf("  apply rule #2");
            int pos = strlen(token) / 2;
            strcpy(app, insert_char_in_string(token, ' ', pos));
            count++;
        }
        else
        {
            if (debug)
                printf("  apply rule #3");
            char *endptr;
            // Convert the string to a long integer
            long long num = strtoll(token, &endptr, 10);
            long long val = num * 2024;
            sprintf(app, "%lld", val);
        }
        if (debug)
            printf(": %s\n", app);

        strcat(result, " ");
        strcat(result, app);

        if (debug)
            printf("result: %s\n", result);

        // strset(app, 0);

        token = strtok(NULL, " ");
    }

    printf("stone count: %d\n", count);

    return result;
}

void blink_step(linked_list *ll, int debug)
{
    if(debug) printf("[blink_step] list first value: '%s', second: '%s',  list length: %d\n", ll->first->value, ll->first->next->value, ll->count);
    node *current_node = ll->first;
    int count = 0;

    while (current_node != NULL)
    {
        if (debug)
            printf("[blink_step] current_node value: %s\n", current_node->value);

        /*
            first applicable rule in this list:

            1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
            2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
            3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.

        */

        char *token = current_node->value;
        if (strcmp(token, "0") == 0)
        {
            if (debug)
                printf("[blink_step]   apply rule #1: ");
            strcpy(current_node->value, "1");
            if(debug) printf("result: %s\n", current_node->value);
        }
        else if (strlen(token) % 2 == 0)
        {
            if (debug)
                printf("[blink_step]   apply rule #2: ");
            int pos = strlen(token) / 2;
            strcpy(current_node->value, insert_char_in_string(current_node->value, ' ', pos));

            // split node
            node *new_node = (node *)malloc(sizeof(node));
            new_node->value = ""; // prima parte del token
            new_node->next = current_node->next;

            current_node->value = ""; // second parte del token
            current_node->next = new_node;

            ll->count++;
            if(debug) printf("result: %s %s\n", current_node->value, current_node->next->value);
        }
        else
        {
            if (debug)
                printf("[blink_step]   apply rule #3: ");
            char *endptr;
            // Convert the string to a long integer
            long long num = strtoll(token, &endptr, 10);
            long long val = num * 2024;
            sprintf(current_node->value, "%lld", val);
            if(debug) printf("result: %s\n", current_node->value);
        }
        
        current_node = current_node->next;
    }
}