#include "utility.h"

int power_of_two(int num)
{
    int result = (int)pow(2, num);
    return result;
}

int power_of_three(int num)
{
    int result = (int)pow(3, num);
    return result;
}

long long convert(char *str)
{
    char* endptr;
    // Convert the string to a long integer
    long long num = strtoll(str, &endptr, 10);
    if (endptr == str)
    {
        printf("No digits were found.\n");
    }
    else if (*endptr != '\0')
    {
        printf("Invalid character: %c\n", *endptr);
    }
    // else
    // {
    //     printf("The number is: %ld\n", num);
    // }
    else if (num == LLONG_MIN || num == LLONG_MAX)
    {
        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE)
            printf("The value provided was out of range\n");
    }

    return num;
}

long long concatenation(long long a, long long b)
{
    long long result = 0;
    char str[64];
    sprintf(str, "%lld%lld", a, b);
    result = convert(str);
    return result;
}

int op_combination(int position, int combination, int base, int max_position)
{
    int result;
    int app = combination;
    for(int i=max_position; i>=position; i--)
    {
        int current = pow(base, i);
        while((app-current) >= 0)
        {
            app -= current;
            if(i==position) result++;
        }
    }
    return result;
}

