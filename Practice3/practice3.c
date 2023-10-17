#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define M 10
struct listWord{
    char *s;
    struct listWord* lw;
};
struct btree_f{
    char *s;
    int coun;
    struct btree_f* left;
    struct btree_f* right;
};
struct btreeCS{
    int coun;
    struct btreeCS* left;
    struct btreeCS* right;
    struct listWord* lw;
};
typedef struct btree_f btrf;
typedef struct btreeCS btrc;

void inputBtDict(btrf**);
void strInBtrf(btrf **, char*);
char* inpWord();
void strToCountInBtrc(btrf **, btrc **, int *);
void strsInBtrc(btrc **, char*, int);
void strInLw(struct listWord **, char *);
void outDictOfBtrc(btrc **, int);
void freeBtrf(btrf *);
void freeLw(struct listWord *);
void freeBtrc(btrc *);

int main(){
    btrf *t = NULL; btrc *nt = NULL;
    inputBtDict(&t);
    int k = 0; //число, которое будет считать количество всего слов
    strToCountInBtrc(&t, &nt, &k);
    outDictOfBtrc(&nt, k);
    freeBtrf(t);
    freeBtrc(nt);
    return 0;
}
//Функция считывания с входного потока слова в бинарное дерево поиска ключом служит слово
void inputBtDict(btrf** t){
    char *word;
    while ((word = inpWord()) != NULL){
        if(isspace(word[0])){
            free(word);
            continue;
        }
        if(word[0] == 0) break;
        strInBtrf(t, word);
        free(word);
    }
}

//Функция, добавляющая слово в бинарное дерево поиска. Если слово есть: увеличивает количество вхождений 
void strInBtrf(btrf **t, char* st) {
    if (*t == NULL) {
        *t = malloc(sizeof(btrf));
        int n = strlen(st)+1;
        (*t)->s = malloc(n);
        strncpy((*t)->s, st, n);
        (*t)->left = (*t)->right = NULL;
        (*t)->coun = 1;
        return;
    }
    if (strcmp(st, (*t)->s)>0) strInBtrf(&((*t)->right), st);
    else if (strcmp(st,(*t)->s)<0) strInBtrf(&((*t)->left),st);
    else (*t)->coun++;
}

//Ввод слова 
char* inpWord(){
    int n = M, k = 0, c = 0;
    char* str = malloc(M);
    if(!str) return NULL;
    c = getchar();
    if(c == EOF){
        free(str);
        return NULL;
    }
    if(ispunct(c) || isspace(c)){
        str[0] = c;
        str[1] = '\0';
        return str;
    }
    while (c != '\n'){
        if(!isalpha(c)){
            ungetc(c, stdin);
            str[k] = '\0';
            return str;
        } 
        if(k == n-2){
            str = realloc(str, k+M);
            n += M;
            if(!str) return NULL;
        }
        str[k] = c;
        k++;
        c = getc(stdin);
    }
    str[k] = '\0';
    return str;
}

//Функция, создающая бинарное дерево поиска по слову в бинарное дерево поиска по вхождению (новое)
void strToCountInBtrc(btrf **t1, btrc **t2, int *k){
    if(*t1 != NULL){
        strToCountInBtrc(&((*t1)->left), t2, k);
        strToCountInBtrc(&((*t1)->right), t2, k);
        *k += (*t1)->coun;
        strsInBtrc(t2, (*t1)->s, (*t1)->coun);
    }
}

//Функция, добавляющее в дерево слово с числом нахождений
void strsInBtrc(btrc **t, char* st, int n){
    if (*t == NULL) {
        *t = malloc(sizeof(btrc));
        (*t)->lw = NULL;
        strInLw(&((*t)->lw), st);
        (*t)->left = (*t)->right = NULL;
        (*t)->coun = n;
        return;
    }
    if ((*t)->coun > n) strsInBtrc(&((*t)->right), st, n);
    else if ((*t)->coun <n) strsInBtrc(&((*t)->left),st, n);
    else strInLw(&((*t)->lw), st);
}

//Функция, добавляющая слово в конец списка
void strInLw(struct listWord **l, char *st){
    if(*l == NULL){
        *l = malloc(sizeof(struct listWord));
        int n = strlen(st)+1;
        (*l)->lw = NULL; (*l)->s = malloc(n);
        strncpy((*l)->s, st, n);
    }
    else strInLw(&((*l)->lw), st);
}

//Вывод дерева поиска, ключом которого являются вхождения (сразу считать), в словарь
void outDictOfBtrc(btrc **t, int k){
    if (*t != NULL){
        outDictOfBtrc(&((*t)->left), k);
        double ratio = (double)((*t)->coun)/k;
        struct listWord *l = (*t)->lw;
        while (l!= NULL){
            printf("%s   %d   %lf\n", l->s, (*t)->coun, ratio);
            l = l->lw;
        }
        outDictOfBtrc(&((*t)->right), k);
    }
}

//Освобождение памяти, отведенного под дерево (разных видов)
void freeBtrf(btrf *t){
    if(t != NULL){
        freeBtrf(t->left);
        freeBtrf(t->right);
        free(t->s);
        free(t);
    }
}
void freeLw(struct listWord *l){
    if(l != NULL){
        freeLw(l->lw);
        free(l->s);
        free(l);
    }
}
void freeBtrc(btrc * t){
    if(t != NULL){
        freeBtrc(t->left);
        freeBtrc(t->right);
        freeLw(t->lw);
        free(t);
    }
}