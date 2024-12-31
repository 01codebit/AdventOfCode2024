#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "..\utility.h"

int main()
{
    char* x = "24908498259";
    char* endptr;

    long long y = strtoll(x, &endptr, 10);
    if (endptr == x)
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
    else if (y == LLONG_MIN || y == LLONG_MAX)
    {
        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE)
            printf("The value provided was out of range\n");
    }

    printf("\ntoken: %s -> value: %lld\n\n", x, y);

    long long a = 123;
    long long b = 456;

    long long r = concatenation(a, b);
    printf("\nconcatenation: %lld || %lld = %lld\n", a, b, r);


    printf("\n");
    for(int pos=2; pos>=0; pos--)
    {
        int op = op_combination(pos, 9, 3, 2);
        printf("pos: %d - op_combination: %d\n", pos, op);
    }
    printf("\n");


    return EXIT_SUCCESS;
}