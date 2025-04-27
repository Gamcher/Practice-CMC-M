#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

int main(){
    int n;
    scanf("%d", &n);
    printf("%d\n", n * 2);
    return 0;
}