#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define dmetasym ">> && || | > < ; &"
#define ometasym "|><;&()"
#define N 100
#define M 100
int c, cQoute1, cQoute2;
char * read_word(char***, int*, int*, int*);

int main(){
    c = getchar();
    while(c != EOF){
        char * word; 
        char ** arr = calloc(M, sizeof(char *)); 
        int sizeArr = M, i = -1;
        while ((c != '\n')&&(c != EOF)){
            int wasMeta = 0;

            char * read_word(char *** arrWords, int *sizeArr, int* index, int* wasMetaS);
            cQoute1 = cQoute2= 0;
            char * word = (char*) calloc(N+1, sizeof(char));
            int i = -1, sizeWord = N;
            char * metaArr = ometasym;
    
            while((c != EOF)&&(c != '\n')){
                if(c == '\''){
                    if(cQoute1 == 1) cQoute1--;
                    else cQoute1++;
                    c = getchar();
                    continue;
            }
            if(c == '\"'){
            if(cQoute2 == 1) cQoute2--;
            else cQoute2++;
            c = getchar();
            continue;
        }
        if((c == '\n')||((c == ' ')&&(cQoute1 == 0)&&(cQoute2 == 0))) {
            if(word[0] != 0){
                break;
            }
            c = getchar();
            continue;
        }
        int wasMeta = 0;
        char mc;
        
        if ((cQoute1 == 0)&&(cQoute2 == 0))
            for (int k=0; k<7; k++)
                if(c == metaArr[k]){
                    char * metaWord = calloc(3, sizeof(char));
                    wasMeta++;
                    metaWord[0] = c;
                    if((c == '>')||(c == '|')||(c == '&')){
                        mc = getchar();
                        wasMeta++;
                        if(c == mc){
                            wasMeta++;
                            metaWord[1] = c;
                        } 
                    }   
                    //здесь уже есть созданный metaWord
                    (*index)++;
                    if (i >= (*sizeArr -1)){
                        *arrWords = (char**) realloc(*arrWords, ((*sizeArr) + M) * sizeof(char*));
                        (*sizeArr) += M;
                    } 
                    if(word[0] != 0){
                        (*arrWords)[(*index)+1] = metaWord;
                        (*wasMetaS)++;
                    }
                    else{
                        (*arrWords)[(*index)] = metaWord;
                    }
                    break;
                }
        if(wasMeta == 0){
            i++;
            if(i >= sizeWord){
                word = (char*) realloc(word, (sizeWord+N) * sizeof(char));
                sizeWord += N;
            }
            word[i] = c; 
            c = getchar();
        }
        else {
            if (wasMeta == 2) c = mc;
            else c = getchar(); 
            break;
        }
    }
    if ((cQoute1 == 1)||(cQoute2 == 1)){
        fprintf(stderr, "Есть неправильное количество кавычек\n");
        free(word);
        return NULL;
    }
    if(i == -1){
        free(word);
        return NULL;
    } 
    word[i+1] = '\0';
    return word;
}
            word = read_word(&arr, &sizeArr, &i, &wasMeta);
            if(word != NULL){
                if (wasMeta == 0){
                    i++;
                    if (i >= sizeArr){
                        arr = (char**) realloc(arr, (sizeArr + M) * sizeof(char*));
                        sizeArr += M;
                    } 
                    arr[i] = word;
                }
                else {
                    arr[i] = word; 
                    i++;
                }
            }
        }
        if ((cQoute1 == 0)&&(cQoute2 == 0)) for (int j = 0; j <= i; j++) printf("%s\n", arr[j]);   
        for (int j = 0; j <= i; j++) free(arr[j]);
        free(arr);
        if(c == EOF) break;
        c = getchar();
    }
}

char * read_word(char *** arrWords, int *sizeArr, int* index, int* wasMetaS){
    cQoute1 = cQoute2= 0;
    char * word = (char*) calloc(N+1, sizeof(char));
    int i = -1, sizeWord = N;
    char * metaArr = ometasym;
    
    while((c != EOF)&&(c != '\n')){
        if(c == '\''){
            if(cQoute1 == 1) cQoute1--;
            else cQoute1++;
            c = getchar();
            continue;
        }
        if(c == '\"'){
            if(cQoute2 == 1) cQoute2--;
            else cQoute2++;
            c = getchar();
            continue;
        }
        if((c == '\n')||((c == ' ')&&(cQoute1 == 0)&&(cQoute2 == 0))) {
            if(word[0] != 0){
                break;
            }
            c = getchar();
            continue;
        }
        int wasMeta = 0;
        char mc;
        
        if ((cQoute1 == 0)&&(cQoute2 == 0))
            for (int k=0; k<7; k++)
                if(c == metaArr[k]){
                    char * metaWord = calloc(3, sizeof(char));
                    wasMeta++;
                    metaWord[0] = c;
                    if((c == '>')||(c == '|')||(c == '&')){
                        mc = getchar();
                        wasMeta++;
                        if(c == mc){
                            wasMeta++;
                            metaWord[1] = c;
                        } 
                    }   
                    //здесь уже есть созданный metaWord
                    (*index)++;
                    if (i >= (*sizeArr -1)){
                        *arrWords = (char**) realloc(*arrWords, ((*sizeArr) + M) * sizeof(char*));
                        (*sizeArr) += M;
                    } 
                    if(word[0] != 0){
                        (*arrWords)[(*index)+1] = metaWord;
                        (*wasMetaS)++;
                    }
                    else{
                        (*arrWords)[(*index)] = metaWord;
                    }
                    break;
                }
        if(wasMeta == 0){
            i++;
            if(i >= sizeWord){
                word = (char*) realloc(word, (sizeWord+N) * sizeof(char));
                sizeWord += N;
            }
            word[i] = c; 
            c = getchar();
        }
        else {
            if (wasMeta == 2) c = mc;
            else c = getchar(); 
            break;
        }
    }
    if ((cQoute1 == 1)||(cQoute2 == 1)){
        fprintf(stderr, "Есть неправильное количество кавычек\n");
        free(word);
        return NULL;
    }
    if(i == -1){
        free(word);
        return NULL;
    } 
    word[i+1] = '\0';
    return word;
}

