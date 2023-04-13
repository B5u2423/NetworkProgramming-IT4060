#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buff[208];
    buff[0] = 'a';
    buff[1] = 'b';
    buff[2] = 0;

    printf("%d\n", strlen(buff));
    return 0;
}