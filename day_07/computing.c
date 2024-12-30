#include "computing.h"
#include <math.h>

int power_of_two(int num)
{
    int result = (int)pow(2, num);
    return result;
}

long long apply_operation(int type, long long a, long long b)
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
    // printf("op_combinations: %d\n", op_combinations);

    for (int comb = 0; comb < op_combinations; comb++)
    {
        long long prev = numbers[0];
        for (int i = 1; i < count; i++)
        {
            int k = comb >> (i - 1);
            int op_type = k & 1;
            if (debug)
            {
                // printf("[%d: %d] ", (i-1), op_type);
                //  printf("%d", op_type);
            }
            long long op_res = apply_operation(op_type, prev, numbers[i]);
            prev = op_res;
            if (prev > total)
            {
                break;
            }
        }
        // if(debug) printf("\n");
        // printf("total? %lld =? %lld\n", prev, total);
        // printf("total is %lld, computed value for combination #%d is %lld\n", total, comb, prev);
        if (prev == total)
        {
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

        long long r = calibration_result(c.data[i].numbers, total, count, debug);
        if (debug && (r > 0)) printf("calibration #%d result: %lld\n", i, r);

        result += r;
    }

    return result;
}