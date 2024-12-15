#include "printer.h"

void print_array(char* title, int* array, int len)
{
    printf("\n%s: ", title);
    for(int i = 0; i<len; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
