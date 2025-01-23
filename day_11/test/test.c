#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *value;
    int count;
} container;

void function(container c)
{
    printf("value: '%s'\n", c.value);
    printf("count: %d\n", c.count);
}


int main(int argc, char *argv[])
{
    container c;
    c.count = 99;
    c.value = "ciao";

    function(c);

    return 0;
}
