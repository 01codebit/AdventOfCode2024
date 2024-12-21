#include "word_searcher.h"

int search_direction(char** matrix, int rows, int cols, char* word, int originRow, int originCol, int directionX, int directionY)
{
    int len = strlen(word);

    int x = originCol;
    int y = originRow;

    for(int i=1; i<len; i++)
    {
        x += directionX;
        y += directionY;

        if(x<0 || x>=cols || y<0 || y>=rows)
        {
            // out of bounds
            return 0;
        }

        char current = matrix[y][x];
        if(current!=word[i])
        {
            // not the word
            return 0;
        }
    }

    return 1;
}


int count_occurrences(char** matrix, int rows, int cols, char* word)
{
    int result = 0;
    for(int row=0; row<rows; row++)
    {
        for(int col=0; col<cols; col++)
        {
            char origin = matrix[row][col];
            if(origin==word[0])
            {
                // printf("found '%c'=='%c' in (%d,%d)\n", origin, word[0], col, row);
                int sum = 0;

                for(int dir_x=-1; dir_x<2; dir_x++)
                {
                    for(int dir_y=-1; dir_y<2; dir_y++)
                    {
                        int dir = search_direction(matrix, rows, cols, word, row, col, dir_x, dir_y);
                        if(dir>0) {
                            // printf("  word in direction(x:%d, y:%d)\n", dir_x, dir_y);
                            sum += dir;
                        }
                    }
                }
                result += sum;
            }
        }
    }

    return result;
}

int check_x_word(char left_up, char right_up, char left_down, char right_down, char alt1, char alt2)
{
    // printf("check_x_word(%c, %c, %c, %c, %c, %c)\n", left_up, right_up, left_down, right_down, alt1, alt2);
    if((left_up==alt1 && right_down==alt2) || (left_up==alt2 && right_down==alt1))
    {
        if((right_up==alt1 && left_down==alt2) || (right_up==alt2 && left_down==alt1))
        {
            return 1;
        }
    }
    return 0;
}


int count_x_occurrences(char** matrix, int rows, int cols, char* word)
{
    int result = 0;
    for(int row=1; row<rows-1; row++)
    {
        for(int col=1; col<cols-1; col++)
        {
            char origin = matrix[row][col];
            if(origin==word[1])
            {
                // printf("found '%c'=='%c' in (%d,%d)\n", origin, word[1], col, row);
                result += check_x_word(matrix[row-1][col-1], 
                                       matrix[row-1][col+1], 
                                       matrix[row+1][col-1], 
                                       matrix[row+1][col+1], 
                                       word[0], word[2]);
            }
        }
    }

    return result;
}