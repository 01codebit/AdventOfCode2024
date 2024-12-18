#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char* PATTERN = "mul(,)";

// search for 'mul(xxx,yyy)'
int parse_string(char* text, int len)
{
    int count = 0;
    int aaa = 0;
    int bbb = 0;
    int result = 0;
    int numbers = 2;

    for(int i=0; i<len; i++)
    {
        char ch = text[i];
        int chi = (int)ch;

        printf("[%d] %c --> count: %d\tresult: %d\n", i, ch, count, result);

        // number
        if(chi>=48 && chi<=57)
        {
            if(numbers<0)
            {
                numbers=2;
                count=0;
                aaa = 0;
                bbb = 0;
            }
            else if(count==4)
            {
                aaa += chi * pow(10, numbers);
                printf("- aaa: %d\n", aaa);
                numbers--;
            }
            else if(count==5)
            {
                bbb += chi * pow(10, numbers);
                printf("- bbb: %d\n", bbb);
                numbers++;
            }
        }

        //  012345
        // "mul(,)"
        if(ch==PATTERN[count])
        {
            count++;
            if(count==4)
            {
                numbers = 0;
            }

            if(count==6)
            {
                result += aaa * bbb;
                aaa = 0;
                bbb = 0;
            }
        }
    }

    return result;
}