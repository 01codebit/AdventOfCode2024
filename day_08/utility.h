#ifndef _UTILS
#define _UTILS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int power_of_two(int num);

long long power_of_three(int num);

long long convert(char *str);

long long concatenation(long long a, long long b);

int op_combination(int position, int combination, int base, int max_position);

#endif