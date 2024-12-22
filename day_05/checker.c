#include "checker.h"

int check_rules(int prev, int next, rule* rules, int rules_count)
{
    int result = 0;
    for(int r=0; r<rules_count; r++)
    {
        rule current_rule = rules[r];
        if(current_rule.next==prev&&current_rule.prev==next)
        result++;
    }

    return result;
}


int evaluate(data input)
{
    printf("evaluate %d rules for %d updates...\n", input.rules_count, input.updates_count);

    int good_updates = 0;
    int middles = 0;

    for(int i=0; i<input.updates_count; i++)
    {
        // printf("  update %d/%d\n", i, input.updates_count);
        update current_update = input.updates[i];
        int errors = 0;
        for(int j=0; j<current_update.size; j++)
        {
            // printf("    page %d/%d\n", j, current_update.size);
            int current_page = current_update.pages[j];
            for(int k=j; k<current_update.size; k++)
            {
                errors += check_rules(current_page, current_update.pages[k], input.rules, input.rules_count);
            }
        }
        if(errors==0) {
            good_updates++;
            int middle = current_update.pages[current_update.size/2];
            // printf("  update[%d] middle: %d\n", i, middle);
            middles += middle;
        }
    }

    // printf("good updates: %d\n", good_updates);
    return middles;
}


int fix_update(update u, rule* rules, int rules_count)
{
    int errors = 1;
    int middle = 0;

    int* pages = u.pages;

    for(int j=0; j<u.size; j++)
    {
        int current_page = pages[j];
        for(int k=j; k<u.size; k++)
        {
            if(check_rules(current_page, pages[k], rules, rules_count)>0)
            {
                // printf("swap %d with %d\n", pages[j], pages[k]);
                pages[j] = pages[k];
                pages[k] = current_page;
                current_page = pages[j];
            }
        }
    }

    middle = pages[u.size/2];
    // printf("middle: %d\n", middle);

    return middle;
}

int evaluate_and_fix(data input)
{
    printf("evaluate_and_fix %d rules for %d updates...\n", input.rules_count, input.updates_count);

    int good_updates = 0;
    int middles = 0;

    for(int i=0; i<input.updates_count; i++)
    {
        // printf("  update %d/%d\n", i, input.updates_count);
        update current_update = input.updates[i];
        int errors = 0;
        for(int j=0; j<current_update.size; j++)
        {
            // printf("    page %d/%d\n", j, current_update.size);
            int current_page = current_update.pages[j];
            for(int k=j; k<current_update.size; k++)
            {
                errors += check_rules(current_page, current_update.pages[k], input.rules, input.rules_count);
            }
        }
        if(errors>0)
        {
            int middle = fix_update(current_update, input.rules, input.rules_count);
            middles += middle;
        }
    }

    // printf("good updates: %d\n", good_updates);
    return middles;
}