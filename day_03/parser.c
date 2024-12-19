#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char* PATTERN = "mul(A,B)";


int isNumber(char ch)
{
    int chi = (int)ch;
    if(chi>=48 && chi<=57) return 1;
    return 0;
}

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

        if(ch=='m' && count==0)
        {
            count++;
        }
        else if(ch=='u' && count==1)
        {
            count++;
        }
        else if(ch=='l' && count==2)
        {
            count++;
        }
        else if(ch=='(' && count==3)
        {
            count++;
        }
        else if(count==4 && isNumber(ch))
        {
            // aaa
            aaa = aaa * 10 + chi-48;
        }
        else if(ch==',' && count==4)
        {
            count++;
        }
        else if(count==5 && isNumber(ch))
        {
            // bbb
            bbb = bbb * 10 + chi-48;
        }
        else if(ch==')' && count==5)
        {
            // calcolo
            result += aaa * bbb;
            count = 0;
            aaa = 0;
            bbb = 0;
        }
        else
        {
            count = 0;
            aaa = 0;
            bbb = 0;
        }

/*

        // number
        if(chi>=48 && chi<=57)
        {
            // printf("is a number\n");
            if(numbers<0)
            {
                // printf("  numbers<0 - count:%d\n", count);
                if(count==5)
                {
                    numbers=2;
                }
                // else if(count==7)
                // {
                //     numbers=2;
                //     count=0;
                //     aaa = 0;
                //     bbb = 0;
                // }
            }
            else if(count==4) // A
            {
                // printf("  count==%d set aaa\n", count);
                aaa = aaa * 10 + chi-48;
                // printf("- aaa: %d\n", aaa);
                numbers--;
                count++;
            }
            else if(count==5) // A
            {
                // printf("  count==%d set aaa\n", count);
                aaa = aaa * 10 + chi-48;
                // printf("- aaa: %d\n", aaa);
                numbers--;
            }
            else if(count==6) // B
            {
                printf("  ch: %c count==%d set bbb\n", ch, count);
                bbb = bbb * 10 + chi-48;
                // printf("- bbb: %d\n", bbb);
                numbers++;
                count++;
            }
            else if(count==7) // B
            {
                printf("  ch: %c count==%d set bbb\n", ch, count);
                bbb = bbb * 10 + chi-48;
                // printf("- bbb: %d\n", bbb);
                numbers++;
            }
        }
        //  01234567
        // "mul(A,B)"
        else if(ch==PATTERN[count])
        {
            // printf("ch in pattern\n");
            if(count==5)
            {
                // printf("  count==%d reset numbers\n", count);
                numbers = 0;
                count++;
            }
            else if(count==7)
            {
                printf("  count==%d compute result aaa:%d * bbb:%d\n", count, aaa, bbb);
                result += aaa * bbb;
                aaa = 0;
                bbb = 0;
                numbers=2;
                count=0;
            }
            else 
            {
                count++;
            }
            if(count>7) count=0;
        }
        else count=0;
*/
    }

    return result;
}