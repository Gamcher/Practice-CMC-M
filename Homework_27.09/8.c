#include <stdio.h>
#include <string.h>

char* strstra(char*,char*);

int main(int argc, char **argv){
    char *str1, *str2;
    int i=1;
    
    while(i<argc){
        str1 = argv[i];
        str2 = strstra(str1, "end");
        while(str2 != NULL){
            str1 = str2;
            str2 = strstra(str2+1, "end");
        }                                                       
        printf("%s\n", str1);
        i++;
    }
    return 0;
}

char* strstra(char* str1, char* str2){
    int k=0;
    int len2 = strlen(str2);
    while (*str1){
        if(*str1 == *str2){
            k++;
            if(k == len2) return str1-k+1;
            str2++;
        }
        else {
            str2 = str2 - k;
            k=0;
        }    
        str1++;
    }
    return NULL;
}