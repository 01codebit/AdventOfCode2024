#include "file_reader.h"

typedef struct {
    rule* data;
    int size;
} rules_struct;

rules_struct read_rules(FILE* input)
{
    char buffer[20];
    int rules_count = 0;

    rule* rules;

    while(fgets(buffer, 20, input))
    {
        if(buffer[0] == '\n') break;

        char* p = strtok(buffer, "|");
        char* n = strtok(NULL, " \t\n\r");

        int pi = atoi(p);
        int ni = atoi(n);

        rule r;
        r.prev = pi;
        r.next = ni;

        if(rules_count==0)
        {
            rules = (rule*)calloc(CHUNK_SIZE, sizeof(rule));
        }
        else if((rules_count % CHUNK_SIZE)==0)
        {
            int new_size = rules_count / CHUNK_SIZE + 1;
            rules = (rule*)realloc(rules, new_size * CHUNK_SIZE * sizeof(rule));
        }

        rules[rules_count] = r;
        rules_count++;
    }

    rules_struct result;
    result.data = rules;
    result.size = rules_count;

    return result;
}

typedef struct {
    update* data;
    int size;
} updates_struct;

updates_struct read_updates(FILE* input)
{
    char* line = (char*)calloc(1024, sizeof(char));
    int updates_count = 0;
    char* token;
    int pages_count = 0;

    update* updates;

    while(fgets(line, 1024, input))
    {
        token = strtok(line, ",");

        if(updates_count==0)
        {
            updates = (update*)calloc(CHUNK_SIZE, sizeof(update));
        }
        else if((updates_count % CHUNK_SIZE)==0)
        {
            int new_size = updates_count + CHUNK_SIZE;
            // printf("  -- realloc updates new_size: %d\n", new_size);
            updates = (update*)realloc(updates, new_size * sizeof(update));
        }

        while(token!=NULL)
        {
            int current_int = atoi(token);

            if(pages_count==0)
            {
                updates[updates_count].pages = (int*)calloc(CHUNK_SIZE, sizeof(int));
            }
            else if((pages_count % CHUNK_SIZE)==0)
            {
                int new_size = pages_count + CHUNK_SIZE;
                updates[updates_count].pages = (int*)realloc(updates[updates_count].pages, new_size * sizeof(int));
            }
            updates[updates_count].pages[pages_count] = current_int;

            pages_count++;
            token = strtok(NULL, ",");
        }

        updates[updates_count].size = pages_count;

        // printf("  updates[%d].size = %d (%d)\n", updates_count, pages_count, updates[updates_count].size);

        updates_count++;

        pages_count = 0;
    }


    updates_struct result;
    result.data = updates;
    result.size = updates_count;

    return result;
}

data read_data(char* filename)
{
    FILE* input = fopen(filename, "r");
    
    data result;
    rules_struct rules;
    updates_struct updates;

    if(input)
    {
        printf("read rules...");
        rules = read_rules(input);
        printf(" read %d rules\n", rules.size);

        printf("read updates...");
        updates = read_updates(input);
        printf(" read %d updates\n", updates.size);
 
        fclose(input);
   }
    else
    {
       fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    result.rules = rules.data;
    result.rules_count = rules.size;
    result.updates = updates.data;
    result.updates_count = updates.size;

    return result;
}