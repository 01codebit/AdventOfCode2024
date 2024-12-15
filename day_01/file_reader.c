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
    int* first_list = (int*)malloc(CHUNK_SIZE * sizeof(int));
    int* second_list = (int*)malloc(CHUNK_SIZE * sizeof(int));

    int count = 0;

    if(input != NULL)
    {
        while (fgets(line, sizeof(line), input))
        {
            char* word1 = strtok(line, " \t\n\r");
            char* word2 = strtok(NULL, " \t\n\r");

            int rest = count % CHUNK_SIZE;
            int chunks = count / CHUNK_SIZE + 1;

            if(count!=0 && rest==0)
            {
                // resize arrays
                first_list = chunk_alloc(first_list, chunks);                
                second_list = chunk_alloc(second_list, chunks);
            }

            // parse strings and add values to arrays
            int elem1 = atoi(word1);
            first_list[count] = elem1;

            int elem2 = atoi(word2);
            second_list[count] = elem2;

            count++;
        }

        fclose(input);
    }
    else
    {
        fprintf(stderr, "Unable to open the file!\n");
    }

    result.array1 = first_list;
    result.array2 = second_list;
    result.size = count;

    return result;
}