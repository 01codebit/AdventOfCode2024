#include "file_reader.h"

rule* read_rules(FILE* input)
{
    rule* rules;

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

        if(rules_count==0)
        {
            rules = (rule*)malloc(CHUNK_SIZE * sizeof(rule));
        }
        else if(rules_count % CHUNK_SIZE==0)
        {
            int new_size = rules_count / CHUNK_SIZE + 1;
            rules = (rule*)realloc(rules, new_size * CHUNK_SIZE * sizeof(rule));
        }

        rules[rules_count] = r;
        rules_count++;
    }

    for(int i=0; i<rules_count; i++)
    {
        printf("rule[%d] prev: %d - next: %d\n", i, rules[i].prev, rules[i].next);
    }

    return rules;
}


data read_data(char* filename)
{
    FILE* input = fopen(filename, "r");
    
    data result;
    rule* rules;
    
    if(input)
    {
        rules = read_rules(input);

//        read_updates(input);
 
        fclose(input);
   }
    else
    {
       fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    result.rules = rules;

    return result;
}