#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int isNumber(char ch)
{
    int chi = (int)ch;
    if(chi>=48 && chi<=57) return 1;
    return 0;
}

// search for 'mul(xxx,yyy)'
int parse_string(char* text)
{
    int len = strlen(text);

    int count = 0;
    int aaa = 0;
    int bbb = 0;
    int result = 0;
    int numbers = 2;

    int doCount = 0;
    int dontCount = 0;

    int enabled = 1;

    for(int i=0; i<len; i++)
    {
        char ch = text[i];
        int chi = (int)ch;

        if(ch=='d' && doCount==0)
        {
            doCount++;
        }
        else if(ch=='o' && doCount==1)
        {
            doCount++;
        }
        else if(ch=='(' && doCount==2)
        {
            doCount++;
        }
        else if(ch==')' && doCount==3)
        {
            doCount = 0;
            enabled = 1;
        }
        else
        {
            doCount = 0;
        }

        if(ch=='d' && dontCount==0)
        {
            dontCount++;
        }
        else if(ch=='o' && dontCount==1)
        {
            dontCount++;
        }
        else if(ch=='n' && dontCount==2)
        {
            dontCount++;
        }
        else if(ch=='\'' && dontCount==3)
        {
            dontCount++;
        }
        else if(ch=='t' && dontCount==4)
        {
            dontCount++;
        }
        else if(ch=='(' && dontCount==5)
        {
            dontCount++;
        }
        else if(ch==')' && dontCount==6)
        {
            dontCount = 0;
            enabled = 0;
        }
        else
        {
            dontCount = 0;
        }

        if(enabled)
        {
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
        }
    }

    return result;
}