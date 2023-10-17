#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct stack{
    char * s;
    struct stack *next;
} stack;
void push(stack ** st, char *s){
    stack *tmp;
    tmp = malloc(sizeof(stack));
    tmp->s = malloc(strlen(s));
    strcpy(tmp->s, s);
    tmp->next = *st;
    (*st) = tmp;
}
void prStackInFile(stack *st, FILE *f){
    if(st!= NULL){
        fputs(st->s, f);
        fputs("\n", f);
        st = st->next;
    }
    while(st != NULL){
        fputs(st->s, f);
        st = st->next;
    }
    return;
}
int main(int argc, char *argv[]){
    char *fileName;
    if (argc > 1){
        fileName = argv[1];
    }
    else {
        printf("Нет имени файла");
        return 1;
    }
    FILE *f = fopen(fileName, "r"); 
    stack *st = NULL;
    char s[256];
    while((fgets(s, 255, f))!=NULL) push(&st, s);
    f = fopen(fileName, "w"); 
    prStackInFile(st, f);
    fclose(f);
    return 0;
}