#include "file_reader.h"

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
                int total = atoi(token);
                new_calibration.total = total;

                int* nums = (int*)malloc(MAX_NUMBERS * sizeof(int));

                token = strtok(NULL, " \t\n\r");
                while (token != NULL)
                {
                    int current_int = atoi(token);
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