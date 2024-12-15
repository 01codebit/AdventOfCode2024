#include "distance.h"


int* order_list(int* list, int len)
{
    for(int i=0; i<len; i++)
    {
        for(int j=i; j<len; j++)
        {
            if(list[i]>list[j])
            {
                int app = list[i];
                list[i] = list[j];
                list[j] = app;
            }
        }
    }
    return list;
}

int distance_sum(int* first, int* second, int len)
{
    int delta = 0;
    for(int i=0; i<len; i++)
    {
        int x = first[i] - second[i];
        if(x<0) x *= -1;
        delta += x;
    }
    return delta;
}

int* distance_sum_array(int* first, int* second, int len)
{
    int* delta = (int*)malloc(len * sizeof(int));
    for(int i=0; i<len; i++)
    {
        int x = first[i] - second[i];
        if(x<0) x *= -1;
        delta[i] = x;
    }
    return delta;
}

int array_sum(int* array, int len)
{
    int sum = 0;
    for(int i=0; i<len; i++)
    {
        sum += array[i];
    }
    return sum;
}
