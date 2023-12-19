#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct btree_f{
    char *s;
    struct btree_f* left;
    struct btree_f* right;
};
void out_btf_inc(struct btree_f*, int);
struct btree_f* in_btf();
int count_sym_in_lay(struct btree_f*, char, int);
void str_in_btf(struct btree_f **, char*);
char *input_str(void);

int main(){
    struct btree_f *t = in_btf();
    int i = 0;
    int n;
    char c;
    printf("%s\n", "Введите символ, который надо будет подсчитать");
    scanf("%c\n", &c);
    printf("%s\n","Введите номер слоя, в котором надо будет найти количество символов" );
    scanf("%d\n", &n);
    printf("%s", "Количество символов в данном слое - ");
    printf("%d\n", count_sym_in_lay(t, c, n));
    printf("%s\n", "По слоям вывод:");
    out_btf_inc(t, i);
}

//Функция вывода дерева поиска по слоям
void out_btf_inc(struct btree_f *t, int i){
    if (t != NULL){
        printf("%s\n", t->s);
        i++;
        
        out_btf_inc(t->left, i);
        out_btf_inc(t->right, i);
    }
}

//Функция ввода дерева поиска по строкам
struct btree_f* in_btf(){
    struct btree_f *t;
    int n;
    printf("%s\n", "Введите количество строк, которые будут в дереве");
    scanf("%d\n", &n);
    for (int i=0; i<n; i++){
        str_in_btf(&t, input_str());
    }
    return t;
}

//Функция добавление одной строки в дерево поиска со строками
void str_in_btf(struct btree_f **t, char* st) {
    if (*t == NULL) {
        *t = malloc(sizeof(struct btree_f));
        (*t)->s = malloc(strlen(st));
        strcpy((*t)->s, st);
        (*t)->left = (*t)->right = NULL;
        return;
    }
     
    if (strcmp(st, (*t)->s)>0) {
        str_in_btf(&((*t)->right), st);
    } else if (strcmp(st,(*t)->s)<0) {
        str_in_btf(&((*t)->left),st);
    }else return;
}

//Функция,которая вычисляет количество вхождений символа на данном слое
int count_sym_in_lay(struct btree_f *t, char c, int n){
    if(t == NULL){
        return 0;
    }
    if(n == 0){ //Слои начинаются с нуля и вниз
        char *p = t->s;
        int count = 0;
        while (*p != 0){
            if(*p == c)
                count += 1;
            p++;
        }
        return count;
    }
    return count_sym_in_lay(t->left, c, n-1) + count_sym_in_lay(t->right, c, n-1); 
}
// Функция, считывающая строку, и возвращающая указатель на неё
char *input_str(void){						
	int n, k=0;							 
	char *s = malloc(10);						 
	if (!s) return NULL;						 
	while (fgets (s+k, 10, stdin)) {
		n = strlen (s);
		if (s[n-1] != '\n') {			
			k = k+10-1;				  	 
			s = realloc (s, k+10);			
			if (!s) return NULL;			
		}
        else {	
            s[n-1] ='\0';
            return s;  
        }   
    }
    free(s);
    return NULL;
}

