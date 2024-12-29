#include "file_reader.h"

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

calibrations read_calibrations(char *filename)
{
    FILE *input = fopen(filename, "r");

    calibrations result;
    calibration *data = (calibration *)malloc(MAX_COUNT * sizeof(calibration));

    int count = 0;

    if (input)
    {
        char line[1024];
        char *token;

        while (fgets(line, 1024, input))
        {
            // first number: total
            token = strtok(line, ":");

            calibration new_calibration;
            new_calibration.count = 0;

            if (token != NULL)
            {
                long long total = convert(token);
                // printf("token: %s -> total: %lld\n", token, total);
                new_calibration.total = total;

                long long *nums = (long long *)malloc(MAX_NUMBERS * sizeof(long long));

                token = strtok(NULL, " \t\n\r");
                while (token != NULL)
                {
                    long long current_int = convert(token);
                    nums[new_calibration.count] = current_int;
                    new_calibration.count++;
                    token = strtok(NULL, " \t\n\r");
                }

                new_calibration.numbers = nums;
                data[count] = new_calibration;
                count++;
            }
        }
        fclose(input);
    }
    else
    {
        fprintf(stderr, "[fopen] unable to open the file '%s': %s [errno:%d]\n", filename, strerror(errno), errno);
    }

    result.data = data;
    result.count = count;

    return result;
}