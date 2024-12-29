#include "computing.h"
#include <math.h>

int power_of_two(int num)
{
    int result = (int) pow(2, num);
    return result;    
}

long long operation(int type, long long a, long long b)
{
    long long result = 0;
    if (type == 0)
    {
        result = a + b;
        // printf("operation: %d + %d = %d\n", a, b, result);
    }
    else if (type == 1)
    {
        result = a * b;
        // printf("operation: %d * %d = %d\n", a, b, result);
    }
    else
    {
        printf("[*** ERROR ***] operation not valid!!!: %d\n", type);
    }

    return result;
}

long long calibration_result(long long *numbers, long long total, int count, int debug)
{
    long long result = 0;
    int op_combinations = power_of_two(count - 1);
    // if(debug) printf("op_combinations: %d\n", op_combinations);

    for (int comb = 0; comb < op_combinations; comb++)
    {
        long long prev = numbers[0];
        for (int i = 1; i < count; i++)
        {
            int k = comb >> (i-1);
            int op_type = k & 1;
            if(debug)
            {
                //printf("[%d: %d] ", (i-1), op_type);
                // printf("%d", op_type);
            }
            long long op_res = operation(op_type, prev, numbers[i]);
            prev = op_res;
        }
        // if(debug) printf("\n");
        // printf("total? %lld =? %lld\n", prev, total);
        if (prev == total)
        {
            // if(debug) printf(" **** FOUND!\n");
            return total;
        }
    }
    // if(debug) printf("\n");
    return result;
}

long long total_calibration_result(calibrations c, int debug)
{
    long long result = 0;

    for (int i = 0; i < c.count; i++)
    {
        long long total = c.data[i].total;
        int count = c.data[i].count;

        result += calibration_result(c.data[i].numbers, total, count, debug);
    }

    return result;
}