#include "main.h"

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;

    if (argc > 1)
    {
        if (strcmp("help", argv[1]) == 0)
        {
            printf("\nProvide arguments: <char* filename>\n\n");
            return 0;
        }
        filename = argv[1];

        if (argc > 2)
        {
            debug = atoi(argv[2]);
            printf("debug: %d\n", debug);
        }
    }

    printf("read file: '%s'\n", filename);

    calibrations m = read_calibrations(filename);
    printf("\nread %d calibrations\n", m.count);

    if (debug)
    {
        printf("\ncalibrations: --------------------------------------------------\n");
        for (int i = 0; i < m.count; i++)
        {
            printf("[%d] %lld: (%d) ", i, m.data[i].total, m.data[i].count);
            for (int j = 0; j < m.data[i].count; j++)
            {
                printf("%lld ", m.data[i].numbers[j]);
            }
            printf("\n");
        }
        printf("----------------------------------------------------------------\n\n");
    }

    long long result = total_calibration_result(m, debug);
    printf("\ntotal calibration result: %lld\n", result);
    printf("\n");

    long long result2 = total_calibration_result_2(m, debug);
    printf("\ntotal calibration result with concatenation: %lld\n", result2);
    printf("\n");

    return 0;
}
