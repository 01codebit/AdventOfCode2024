#include "main.h"


void compare_array(LLONG* ids_1, LLONG* ids_2, int count)
{
    int found = 0;
    for(int i=0; i<count; i++)
    {
        if(ids_1[i] != ids_2[i])
        {
            printf("ERROR: found %d for file id %d (must be %d)\n", ids_2[i], i, ids_1[i]);
            found = 1;
        }
    }
    if(!found) printf("SUCCESS: compare OK\n");
}

LLONG * count_occurrences(LLONG * array, int count, int max_file_id)
{
    LLONG* result = (LLONG*)malloc((max_file_id+1) * sizeof(LLONG));
    for(int i=0; i<=max_file_id; i++)
    {
        result[i] = 0;
    }
    for(int i=0; i<count; i++)
    {
        result[array[i]] += 1;
    }

    return result;
}

int main(int argc, char *argv[])
{
    // defaults
    char *filename = "example.txt";
    int debug = 0;

    char filter = ' ';

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

    printf("[main] read file: '%s'\n", filename);

    disk_map m = read_disk_map(filename);
    if (debug)
    {
        printf("[main] read disk map (length: %d):\n", m.length);
        for (int i = 0; i < m.length; i++)
        {
            printf(" %d", m.map[i]);
        }
        printf("\n");
    }

    // char *ex = expand_disk_map(m);
    expansion ex = expand_disk_map(m);
    if (debug)
    {
        printf("[main] expansion (length: %d):\n", ex.length);
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }
    LLONG* occ_1 = count_occurrences(ex.locations, ex.length, ex.max_file_id);
    int occ_count = 0;
    for(int i=0; i<=ex.max_file_id; i++)
    {
        // printf("file_id: %lld\toccurrences: %lld\n", i, occ_1[i]);
        occ_count += occ_1[i];
    }


    int expanded_free_count = 0;
    FILE *output = fopen("expanded.txt", "w");
    if (output == NULL)
        printf("[main] ERROR opening file expanded.txt\n");
    else
    {
        for (int i = 0; i < ex.length; i++)
        {
            // if(ex.locations[i]<0)
            //     fprintf(output, ".,");
            // else
            //     fprintf(output, "%d,", ex.locations[i]);
            fprintf(output, " %d", ex.locations[i]);
            if(ex.locations[i]<0) expanded_free_count++;
        }
        fprintf(output, "\n");
        fclose(output);
    }

    printf("expanded_free_count: %d\n", expanded_free_count);
    printf("expanded_free_count + used: %d must be %d\n", expanded_free_count + occ_count, ex.length);



    // PART 1 -------------------------------------------------------- begin
    // ULLONG checksum_1 = arrange_expansion(ex);
    // printf("\n[main] checksum_1: %lld\n", checksum_1);
    // if (debug)
    // {
    //     printf("[main] arranged expansion (length: %d):\n", ex.length);
    //     for (int i = 0; i < ex.length; i++)
    //     {
    //         printf(" %d", ex.locations[i]);
    //     }
    //     printf("\n");
    // }
    // PART 1 -------------------------------------------------------- end

    // PART 2 -------------------------------------------------------- begin
    arrange_expansion_n(ex);
    if (debug)
    {
        printf("[main] arranged expansion (length: %d):\n", ex.length);
        for (int i = 0; i < ex.length; i++)
        {
            printf(" %d", ex.locations[i]);
        }
        printf("\n");
    }
    // if (debug)
    {
        int arranged_free_count = 0;
        FILE *output = fopen("arranged.txt", "w");
        if (output == NULL)
            printf("[main] ERROR opening file arranged.txt\n");
        else
        {
            for (int i = 0; i < ex.length; i++)
            {
                fprintf(output, " %d", ex.locations[i]);
                if(ex.locations[i]<0) arranged_free_count++;
            }
            fprintf(output, "\n");
            fclose(output);
        }
        printf("arranged_free_count: %d\n", arranged_free_count);
    }

    LLONG* occ_2 = count_occurrences(ex.locations, ex.length, ex.max_file_id);
    int occ_count_2 = 0;
    for(int i=0; i<=ex.max_file_id; i++)
    {
        // printf("file_id: %lld\toccurrences: %lld\n", i, occ_2[i]);
        occ_count_2 += occ_2[i];
    }

    compare_array(occ_1, occ_2, ex.max_file_id+1);


    // PART 2 -------------------------------------------------------- end

    LLONG checksum = compute_checksum(ex);
    printf("\n[main] checksum: %lld", checksum);

    if (checksum >= 6307653502443)
        printf(" <--- ERROR: checksum is too high!\n"); // first try result
    else if (checksum <= 6307371651323)
        printf(" <--- ERROR: checksum is too low!\n"); // second try result
    else
        printf("\n");
    
    printf("\n");

    // free used memory
    free(ex.locations);
    free(m.map);

    return 0;
}
