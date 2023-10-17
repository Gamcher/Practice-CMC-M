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
void prStack(stack *st){
    while(st != NULL){
        printf("%s", st->s);
        st = st->next;
    }
    return;
}
int main(int argc, char *argv[]){
    FILE *f = fopen("./file1.txt", "r");
    stack *st =NULL;
    char *s;
    while((s=fgets(s, 255, f))!=NULL) if(strlen(s)<=7) push(&st, s);
    if(st!= NULL){
        printf("%s\n", st->s);
        st = st->next;
    }
    while(st != NULL){
        printf("%s", st->s);
        st = st->next;
    }
    fclose(f);
    return 0;
}