#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    char *fFName, *sFName;
    if (argc > 2){
        fFName = argv[1];
        sFName = argv[2];
    } else {
        printf("Нет имени файла");
        return 1;
    }
    FILE *f1 = fopen(fFName, "r");
    FILE *f2 = fopen(sFName, "w");   
    char s[255];
    while((fgets(s, 255, f1))!=NULL){
        if(s[strlen(s)-1]!='\n'){
            fprintf(f2, "%s\n%s", s, s);
        }
        else fprintf(f2, "%s%s", s, s);
    }    
    fclose(f1);
    fclose(f2);
    return 0;
}