#include "safe_calculator.h"
#include <math.h>
#include <stdio.h>

// returns 0 if report is safe
int is_safe(int* report, int levels, int bad_level)
{
    if(levels<2) return 1;

    int prev = report[0];
    int current; 
    int sign = 0;
    int result = 0;
    int start = 1;

    if(bad_level==0)
    {
        prev = report[1];
        start++;
    }

    for(int l=start; l<levels; l++)
    {
        if(l==bad_level) {
            continue;
        }

        current = report[l];

        if(sign==0)
        {
            sign = (current-prev > 0) ? 1 : -1;
        } 
        else
        {
            if((current-prev > 0 && sign==-1) || (current-prev < 0 && sign==1))
            {
                result++;
            }
        }

        int delta = abs(current-prev);
        if(delta<1 | delta>3)
        {
            result++;
        }

        prev = current;
    }

    return result;
}

int total_safe_count(int** data, int reports, int* levels)
{
    int result = 0;
    for(int r=0; r<reports; r++)
    {
        int x = 99;
        for(int bad_level=-1; bad_level<levels[r]; bad_level++)
        {
            int y = is_safe(data[r], levels[r], bad_level);
            if(x>y) x = y;
        }

        if(x==0) { 
            result++;
        }
    }
    return result;
}
