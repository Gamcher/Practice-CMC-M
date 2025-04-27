#include <stdio.h>
#include <stdlib.h>

void hocus_pocus(int *param1, int *param2)
{
    int v3;
    v3 = *param1;
    *param1 = *param2;
    *param2 = v3;
}

unsigned int holy_function(int a, int b)
{
    int v3, v4, v5;
    v4 = a;
    v3 = b;
    v5 = a * b;
    if(a < b)
        hocus_pocus(&v4, &v3);
    while(v3)
    {
        v4 %= v3;
        hocus_pocus(&v4, &v3);
    }
    return (unsigned int)(v5/v4);
}

int main(){
    int a, b;
    unsigned int NOK;
    scanf("%d %d", &a, &b);
    NOK = holy_function(a, b);
    printf("%d\n", NOK);
    return 0;
}