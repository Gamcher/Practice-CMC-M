#include <stdio.h>
 
int main(void)
{
    int M, N, K, i;
 
    scanf("%d%d", &M, &N);
    if (!M || !N)
    {
        printf("Error\n");
        return 1;
    }
 
    K = N * M;
    for (i = K; i > 0; i--)
        if (i % M == 0 && i % N == 0)
            K = i;
 
    printf("%d\n", K);
    return 0;
}