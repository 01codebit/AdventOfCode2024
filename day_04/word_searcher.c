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
            printf("FALSE found one word! start: (x:%d,y:%d) direction: (x:%d, y:%d)\n", originCol, originRow, directionX, directionY);
            // out of bounds
            return 0;
        }

        char current = matrix[y][x];
        if(current!=word[i])
        {
            printf("FALSE found one word! start: (x:%d,y:%d) direction: (x:%d, y:%d)\n", originCol, originRow, directionX, directionY);
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
                printf("found '%c'=='%c' in (%d,%d)\n", origin, word[0], col, row);

                int east = search_direction(matrix, rows, cols, word, row, col, 1, 0);
                int south_east = search_direction(matrix, rows, cols, word, row, col, 1, 1);
                int south = search_direction(matrix, rows, cols, word, row, col, 0, 1);
                int south_west = search_direction(matrix, rows, cols, word, row, col, -1, 1);
                int west = search_direction(matrix, rows, cols, word, row, col, -1, 0);
                int north_west = search_direction(matrix, rows, cols, word, row, col, -1, -1);
                int north = search_direction(matrix, rows, cols, word, row, col, 0, -1);
                int north_east = search_direction(matrix, rows, cols, word, row, col, -1, 1);

                int sum = east + south_east + south + south_west + west + north_west + north + north_east;

                result += sum;
            }
        }
    }

    return result;
}
