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
        result = -1;
    }

    return result;
}

long long calibration_result_2(long long *numbers, long long total, int count, int debug, int calibration_n)
{
    long long result = 0;
    int positions = count - 1;
    long long op_combinations = power_of_three(positions);
    int errors = 0;
    // printf("op_combinations: 3^%d = %d\n", count - 1, op_combinations);

    for (int comb = 0; comb < op_combinations; comb++)
    {
        long long prev = numbers[0];
        if (debug)
            printf("comb %d/%d: ", comb, op_combinations);
        for (int i = 1; i < count; i++)
        {
            int op_type = op_combination(i - 1, comb, 3, positions);

            if (debug)
            {
                printf("[%d: %d] ", (i-1), op_type);
            }

            long long op_res = apply_operation_3(op_type, prev, numbers[i]);
            if (op_res < 0)
            {
                errors++;
            }
            else
            {
                prev = op_res;
                if (prev > total)
                {
                    break;
                }
            }
        }
        if (debug) printf("\n");

        if (prev == total)
        {
            return total;
        }
    }

    if (errors > 0)
    {
        printf("calibration #%d: %d errors\n", calibration_n, errors);
        result = -1;
    }
    return result;
}

long long total_calibration_result_2(calibrations c, int debug)
{
    long long result = 0;

    for (int i = 0; i < c.count; i++)
    {
        long long total = c.data[i].total;
        int count = c.data[i].count;

        long long r = calibration_result_2(c.data[i].numbers, total, count, debug, i);
        if (debug && (r > 0))
            printf("calibration #%d result: %lld\n", i, r);

        if (r < 0)
            break;

        result += r;
    }

    return result;
}