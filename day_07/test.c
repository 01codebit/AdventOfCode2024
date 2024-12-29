#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int test()
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


    return EXIT_SUCCESS;
}