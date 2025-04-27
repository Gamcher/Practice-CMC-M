#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(){
    FILE *fd = fopen("input", "w");
    int d = 42.47;
    fwrite(&d, 8, 1, fd);
    char *str2 = "11111111admin\0";
    fwrite(str2, strlen(str2), 1, fd);
    fclose(fd);
}