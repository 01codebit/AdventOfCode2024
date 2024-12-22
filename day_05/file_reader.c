#include "file_reader.h"

int read_rules(FILE* input, rule* rules)
{
    char buffer[20];
    int rules_count = 0;

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

        if((rules_count % CHUNK_SIZE)==0)
        {
            int new_size = rules_count / CHUNK_SIZE + 1;
            rules = (rule*)realloc(rules, new_size * CHUNK_SIZE * sizeof(rule));
        }

        rules[rules_count] = r;
        rules_count++;
    }

    return rules_count;
}

int read_updates(FILE* input, update* updates)
{
    char line[256];
    int updates_count = 0;
    char* token;
    int pages_count = 0;

    while(fgets(line, 20, input))
    {
        pages_count = 0;
        token = strtok(line, ",");
        while(token!=NULL)
        {
            int current_int = atoi(token);

            if(pages_count==0)
            {
                updates[updates_count].chapters = (int*)malloc(CHUNK_SIZE * sizeof(int));
            }
            else if((pages_count % CHUNK_SIZE)==0)
            {
                int new_size = pages_count % CHUNK_SIZE + 1;
                updates[updates_count].chapters = (int*)realloc(updates[updates_count].chapters, new_size * CHUNK_SIZE * sizeof(int));
            }
            updates[updates_count].chapters[pages_count] = current_int;

            pages_count++;
            token = strtok(NULL, ",");
        }
        updates[updates_count].size = pages_count;
        updates_count++;
    }

    return updates_count;
}

data read_data(char* filename)
{
    FILE* input = fopen(filename, "r");
    
    data result;
    rule* rules;
    int rules_count = 0;
    update* updates;
    int updates_count = 0;

    if(input)
    {
        rules = (rule*)malloc(CHUNK_SIZE * sizeof(rule));
        rules_count = read_rules(input, rules);

        updates = (update*)malloc(CHUNK_SIZE * sizeof(update));
        updates_count = read_updates(input, updates);
 
        fclose(input);
   }
    else
    {
       fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    result.rules = rules;
    result.rules_count = rules_count;
    result.updates = updates;
    result.updates_count = updates_count;

    return result;
}