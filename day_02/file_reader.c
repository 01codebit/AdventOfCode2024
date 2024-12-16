#include "file_reader.h"

int* chunk_alloc(int* array, int new_size)
{
    int* new_array = (int*)realloc(array, new_size * CHUNK_SIZE * sizeof(int));
    return new_array;
}

data read_file(char* filename)
{
    FILE* input = fopen(filename, "r");

    // Buffer to store each line of the file.
    char line[LINE_LEN];

    data result;
    int** reports_array = (int**)malloc(CHUNK_SIZE * sizeof(int*));
    int* levels_counts = (int*)malloc(CHUNK_SIZE * sizeof(int));

    int reports = 0;
    int levels = 0;

    if(input != NULL)
    {
        while (fgets(line, sizeof(line), input))
        {
            int* current_report = (int*)malloc(CHUNK_SIZE * sizeof(int));
            levels = 0;
            char* token = strtok(line, " \t\n\r");
            while(token!=NULL)
            {
                // printf("read token: %s\n", token);
                int elem = atoi(token);
                current_report[levels] = elem;
                levels++;
                
                int rest = levels % CHUNK_SIZE;
                int chunks = levels / CHUNK_SIZE + 1;
                if(levels!=0 && rest==0)
                {
                    // resize array
                    current_report = chunk_alloc(current_report, chunks);
                }

                token = strtok(NULL, " \t\n\r");
            }
            // printf("read %d levels\n", levels);

            int rest2 = reports % CHUNK_SIZE;
            int chunks2 = reports / CHUNK_SIZE + 1;
            if(reports!=0 && rest2==0)
            {
                // resize arrays
                reports_array = (int**)realloc(reports_array, chunks2 * CHUNK_SIZE * sizeof(int*));
                levels_counts = chunk_alloc(levels_counts, chunks2);
            }
            reports_array[reports] = current_report;
            levels_counts[reports] = levels;

            reports++;
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }

    result.array = reports_array;
    result.reports_count = reports;
    result.levels_counts = levels_counts;

    return result;
}