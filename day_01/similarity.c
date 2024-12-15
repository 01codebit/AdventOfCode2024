#include "similarity.h"

int similarity_score(int* first_list, int* second_list, int size)
{
    int result = 0;

    for(int i=0; i<size; i++)
    {
        int current = first_list[i];
        int current_count = 0;
        
        for(int j=0; j<size; j++)
        {
            if(second_list[j]==current)
            {
                current_count++;
            }
        }

        result += current * current_count;
    }

    return result;
}