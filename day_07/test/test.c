#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "..\utility.h"

int main(int argc, char** argv)
{
    int input = 0;
    if(argc==2)
    {
        char* arg = argv[1];
        input = atoi(arg);
    }

    printf("\n");
    int combination_number = input>0 ? input : 9;
    int max_positions = 8;
    int base = 3;
    printf("number: %d\n\n", combination_number);
    printf("max number: %d^%d = %lld\n\n", base, max_positions, power_of_three(max_positions));

    for(int pos=max_positions-1; pos>=0; pos--)
    {
        int op = op_combination(pos, combination_number, base, max_positions);
        int x = power_of_three(pos) * op;
        printf("%2d", op);
    }
    printf("\n\n");

    int result = 0;
    for(int pos=max_positions-1; pos>=0; pos--)
    {
        int op = op_combination(pos, combination_number, base, max_positions);
        int x = power_of_three(pos) * op;
        result += x;
        printf("[%2d] op_combination: %d * %d^%d = %d\n", pos, op, base, pos, x);
    }
    printf("\nresult: %d\n", result);
    printf("\n");


    return EXIT_SUCCESS;
}