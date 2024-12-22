#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* array;
    int size;
} data;

data function(int* array)
{
    int len = 256;
    for(int i=0; i<len; i++)
    {
        if(i>128)
        {
            array = (int*)realloc(array, 256*sizeof(int));
        }
        array[i] = i;
    }

    data d;
    d.array = array;
    d.size = len;

    return d;
}

data function2(int* array)
{
    int len = 10;
    for(int i=0; i<len; i++)
    {
        array[i] = i;
    }

    data d;
    d.array = array;
    d.size = len;

    return d;
}


int main (int argc, char** argv)
{
    int* array;
    array = (int*)malloc(128*sizeof(int));
    data l = function(array);

    printf("len: %d - array:", l.size);
    for(int i=0; i<l.size; i++)
    {
        printf(" %d", array[i]);
        printf(" (%d)", l.array[i]);
    }
    printf("\n");

    return 0;
}