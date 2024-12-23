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

void print_matrix(char** data, int rows, int cols)
{
    printf("\n");
    for(int r=0; r<rows; r++)
    {
        for(int c=0; c<cols; c++)
        {
            printf("%c ", data[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrix_marker(char** data, int rows, int cols, char marker, int marker_x, int marker_y)
{
    printf("\n");
    for(int r=0; r<rows; r++)
    {
        for(int c=0; c<cols; c++)
        {
            if(r==marker_y && c==marker_x)
            {
                printf("%c ", marker);
            }
            else
            {
                printf("%c ", data[r][c]);
            }
        }
        printf("\n");
    }
    printf("\n");
}
