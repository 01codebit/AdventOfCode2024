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

void print_int_matrix(int** data, int rows, int cols)
{
    printf("\n");
    for(int r=0; r<rows; r++)
    {
        for(int c=0; c<cols; c++)
        {
            printf(" %d", data[r][c]);
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

int print_matrix_markers(char** data, int rows, int cols, char marker, int* marker_x, int* marker_y, int markers_count)
{
    printf("\n");
    int unique_count = 0;
    for(int r=0; r<rows; r++)
    {
        for(int c=0; c<cols; c++)
        {
            int found = 0;
            for(int k=0; k<markers_count; k++)
            {
                if(r==marker_y[k] && c==marker_x[k])
                {
                    printf("%c ", marker);
                    found = 1;
                    unique_count++;
                    break;
                }
            }
            
            if(!found)
            {
                printf("%c ", data[r][c]);
            }
        }
        printf("\n");
    }
    printf("\n");

    return unique_count;
}
