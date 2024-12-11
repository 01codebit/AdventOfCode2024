#include <stdio.h>
#include <stdlib.h>

#define LIST_LEN 6

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

void print_array(char* title, int* array, int len)
{
    printf("\n%s: ", title);
    for(int i = 0; i<len; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}


int main()
{
    int first[LIST_LEN] = {3, 4, 2, 1, 3, 3};
    int second[LIST_LEN] = {4, 3, 5, 3, 9, 3};

    // azzerare il contatore
    
    // finchè le liste non sono vuote:

    //   estrarre il numero più piccolo della prima lista
    //   estrarre il numero più piccolo della seconda lista
    //   salvare la loro differenza nel contatore

    // stampare il risultato

    order_list(first, LIST_LEN);
    print_array("ordered first list", first, LIST_LEN);

    order_list(second, LIST_LEN);
    print_array("ordered second list", second, LIST_LEN);

    // int delta = distance_sum(first, second, LIST_LEN);
    // printf("\ndelta: %d\n", delta);

    int* delta_array = distance_sum_array(first, second, LIST_LEN);
    print_array("delta list", delta_array, LIST_LEN);

    printf("\ntotal delta: %d\n", array_sum(delta_array, LIST_LEN));

    return 0;
}
