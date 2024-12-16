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

    // printf("report: %d ", prev);
    for(int l=1; l<levels; l++)
    {
        if(l==bad_level) {
            continue;
        }

        current = report[l];
        // printf("%d ", current);

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

        // printf("prev: %d - current: %d - sign: %d - result: %d\n", prev, current, sign, result);
        prev = current;
    }
    // printf("\n");

    return result;
}

int next_bad_level(int* report, int levels, int prev_bad_level)
{
    if(levels<2) return 0;

    int prev = report[0];
    int current; 
    int sign = 0;

    for(int l=1; l<levels; l++)
    {
        current = report[l];

        if(sign==0)
        {
            sign = (current-prev > 0) ? 1 : -1;
        } 
        else
        {
            if((current-prev > 0 && sign==-1) || (current-prev < 0 && sign==1))
            {
                if(l>prev_bad_level) return l;
            }
        }

        int delta = abs(current-prev);
        if(delta<1 | delta>3)
        {
            if(l>prev_bad_level) return l;
        }

        // printf("prev: %d - current: %d - sign: %d\n", prev, current, sign);
        prev = current;
    }

    return 0;
}

int total_safe_count(int** data, int reports, int* levels)
{
    int result = 0;
    for(int r=0; r<reports; r++)
    {
        int bad_level = next_bad_level(data[r], levels[r], -1);
        int x = is_safe(data[r], levels[r], bad_level);
        // printf("report: %d  bad_level: %d  safe: %d\n", r, bad_level, x);
        while(bad_level>0)
        {
            bad_level = next_bad_level(data[r], levels[r], bad_level);
            int y = is_safe(data[r], levels[r], bad_level);

            if(x>y)
            {
                // if(y==0) printf("report[%d] switch results: old:%d new:%d\n", r, x, y);
                x = y;
            }

            // printf("report: %d  bad_level: %d  y: %d  safe: %d\n", r, bad_level, y, x);
        }
        // printf("report[%d] is_safe: %d\n", r, x);
        if(x==0) { 
            // printf("report #%d is safe\n", r);
            result++;
        }
    }
    return result;
}
