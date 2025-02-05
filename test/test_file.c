#include <stdio.h>
#include <stdlib.h>


void save(long long * array, int size)
{
    FILE *output = fopen("output.txt", "w");

    for (int i = 0; i < size; i++)
    {
        if(i%1 == 0) printf("writing... [%d/%d]\r", i, size);
        fprintf(output, "%lld ", array[i]);
    }
    printf("\r                             \n");

    fclose(output);
}


int main(int argc, char **argv)
{
    int size = 200000;

    long long *array;
    printf("initializing array of %lld elements of type long long int (%d B)...\n", size, sizeof(long long));
    array = (long long *)malloc(size * sizeof(long long));

    char size_str[20];
    int bb = size * sizeof(long long);
    if (bb >= (1024*1024))
        sprintf(size_str, "%.2f MB", (float)bb/(1024*1024));
    else if (bb >= 1024)
        sprintf(size_str, "%.2f kB", (float)bb/1024);
    else
        sprintf(size_str, "%d B", bb);

    printf("total bytes on memory: %s\n", size_str);

    for (int i = 0; i < size; i++)
    {
        array[i] = i;
    }

    printf("try to save array...\n");
    save(array, size);

    printf("done\n");

    return 0;
}