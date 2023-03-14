#include <stdio.h>
#include <string.h>

void    tmp(char **a)
{
    *a = strdup("hello");
    (*a)++;
}

int main(void)
{
    char *a;

    tmp(&a);
    printf("%s\n", a);
    return(0);
}