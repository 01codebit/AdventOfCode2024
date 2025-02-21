#include "computing.h"

linked_list *convert_to_linked_list(const char *str, int debug)
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->count = 0;
    list->first = NULL;

    char* str2;//[2048];
    strcpy(str2, str);
    char *token = strtok(str2, " ");
    char *end_ptr;
    long long val = strtoll(token, &end_ptr, 10);

    node *nd = (node *)malloc(sizeof(node));
    // printf("[convert_to_linked_list] first node value: %lld\n", val);
    nd->value = val;

    list->first = nd;
    node *prev = nd;

    while (token)
    {
        if (list->count > 0)
        {
            node *nd = (node *)malloc(sizeof(node));
            // printf("[convert_to_linked_list] #%lld node value: %lld", list->count, val);
            nd->value = val;
            prev->next = nd;
            // list->pre_last = prev;
            prev = nd;
        }

        list->count++;
        token = strtok(NULL, " ");
        val = strtoll(token, &end_ptr, 10);
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

void blink_step(linked_list **llp, int debug)
{
    linked_list *ll = *llp;

    node *current_node = ll->first;
    int count = 0;

    while (current_node != NULL)
    {
        if (debug)
            printf("[blink_step] current_node value: %lld\n", current_node->value);

        /*
            first applicable rule in this list:

            1) If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
            2) If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
            3) If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.

        */

        long long val = current_node->value;
        char *end_ptr;
        char val_str[50];
        sprintf(val_str, "%lld", val);

        if (val == 0)
        {
            if (debug)
                printf("[blink_step]   apply rule #1: ");
            current_node->value = 1;
            if (debug)
                printf("result: %lld\n", current_node->value);

            current_node = current_node->next;
        }
        else if (strlen(val_str) > 1 && strlen(val_str) % 2 == 0)
        {
            if (debug)
                printf("[blink_step]   apply rule #2: ");
            int pos = strlen(val_str) / 2;
            strcpy(val_str, insert_char_in_string(val_str, ' ', pos));

            char *first = strtok(val_str, " ");
            char *second = strtok(NULL, " ");

            // split node
            node *new_node = (node *)malloc(sizeof(node));
            new_node->value = strtoll(second, &end_ptr, 10); // seconda parte del token
            new_node->next = current_node->next;

            current_node->value = strtoll(first, &end_ptr, 10); // prima parte del token
            current_node->next = new_node;

            ll->count++;
            if (debug)
                printf("result: %lld %lld\n", current_node->value, current_node->next->value);

            current_node = current_node->next->next;
        }
        else
        {
            if (debug)
                printf("[blink_step]   apply rule #3: ");
            char *endptr;
            // Convert the string to a long integer
            current_node->value = val * 2024;
            if (debug)
                printf("result: %lld\n", current_node->value);

            current_node = current_node->next;
        }
    }
}