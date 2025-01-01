#include "computing_2.h"

long long apply_operation_3(int type, long long a, long long b)
{
    long long result = 0;
    if (type == 0)
    {
        result = a + b;
        // printf("operation: %d + %d = %d\n", a, b, result);
        // printf("+ ");
    }
    else if (type == 1)
    {
        result = a * b;
        // printf("operation: %d * %d = %d\n", a, b, result);
        // printf("* ");
    }
    else if (type == 2)
    {
        result = concatenation(a, b);
        // printf("operation: %d || %d = %d\n", a, b, result);
        // printf("|| ");
    }
    else
    {
        printf("[*** ERROR ***] operation not valid!!!: %d\n", type);
    }

    return result;
}

long long calibration_result_2(long long *numbers, long long total, int count, int debug)
{
    long long result = 0;
    int positions = count - 1;
    long long op_combinations = power_of_three(positions);
    // printf("op_combinations: 3^%d = %d\n", count - 1, op_combinations);

    for (int comb = 0; comb < op_combinations; comb++)
    {
        long long prev = numbers[0];
        // printf("comb %d/%d: ", comb, op_combinations);
        for (int i = 1; i < count; i++)
        {
            int op_type = op_combination(i-1, comb, 3, positions);

            if (debug)
            {
                printf("[%d: %d] ", (i-1), op_type);
                //  printf("%d", op_type);
            }
            // printf("%lld ", prev);
            long long op_res = apply_operation_3(op_type, prev, numbers[i]);
            // printf("%lld ", numbers[i]);
            prev = op_res;
            if (prev > total)
            {
                break;
            }
        }
        // printf("= %lld (total: %lld)\n", prev, total);
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

long long total_calibration_result_2(calibrations c, int debug)
{
    long long result = 0;

    for (int i = 0; i < c.count; i++)
    {
        long long total = c.data[i].total;
        int count = c.data[i].count;

        long long r = calibration_result_2(c.data[i].numbers, total, count, debug);
        if (debug && (r > 0))
            printf("calibration #%d result: %lld\n", i, r);

        result += r;
    }

    return result;
}