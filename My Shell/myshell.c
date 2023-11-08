#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_ARR 10
#define SIZE_WORD 10
char * metaArr = "|><;&()"; //массив метасимволов

char ** readStr(int *, int *, FILE *);
void freeStr(char **);
void printArr(char **);

int main(int argc, char ** argv){
    //работа с файлом для ввода
    FILE *i;
    if (argc > 1)
        i = fopen(argv[1], "r");
    else  i = stdin;
    //работа со строками
    int c = getc(i);
    while(c != EOF){
        int errorStr = 0; //ошибки помогают понять, есть ли проблемы с кавычками
        char ** arrWords = readStr(&c, &errorStr, i); //считывание построчно
        if(arrWords != NULL){
            //вывод слов строки
            printArr(arrWords);
            freeStr(arrWords);//функция освобождения памяти
            free(arrWords); 
        }
        else
            if(errorStr != 0) fprintf(stderr, "Есть неправильное количество кавычек\n");
        if(c == EOF) break;
        else c = getc(i);
    }
    fclose(i);
}

char ** readStr(int *c, int *errorStr, FILE *i){ //возвращает готовый массив слов
    char ** arr = (char **) malloc(SIZE_ARR * sizeof(char *)); 
    int sizeArr = SIZE_ARR, indexStr = 0;
    arr[0] = NULL;
    int cQoute1 = 0; int cQoute2= 0;
    char * word = NULL; //записываем слово только, когда символ есть для него
    int indexW = 0; 
    int sizeWord = 0;
    char * metaWord = NULL; //это метаслово
    int sizeMetaWord = 0;
    int mc;
    int wasMeta = 0;
    while(((cQoute1+cQoute2 > 0)||(*c != '\n'))&&(*c != EOF)){ //чтение строки до EOF или перехода на след. строку
        //printf("Символы ---> %c\n", *c);
        if(*c == '\''){
            if(cQoute1 == 1) cQoute1--;
            else cQoute1++;
            *c = getc(i);
            continue;
        }
        if(*c == '\"'){
            if(cQoute2 == 1) cQoute2--;
            else cQoute2++;
            *c = getc(i);
            continue;
        }
        if((*c == ' ')&&(cQoute1+cQoute2 == 0)){ //заходим пробел, но без кавычек
            //printf("Я сюда зашел\n");
            if(word != NULL){ //если не равен нулю, то добавляем слово
                if(indexStr == sizeArr-1){ //отслеживаем, чтобы хватало на NULL и слово
                    sizeArr += SIZE_ARR;
                    arr = (char **) realloc(arr, sizeArr * sizeof(char *));
                }
                word[indexW] = 0;
                arr[indexStr] = word;
                indexStr++;
                arr[indexStr] = NULL;
                word = NULL; //возвращаем в начальное состояние слово
                indexW = 0;
                sizeWord = 0; 
            }
            *c = getc(i);
            continue;
        }
        else{ //символ не кавычки, не пробел(без режима кавычек, также не пропуск)
            if ((cQoute1 == 0)&&(cQoute2 == 0)){ //проверка есть ли у нас правило кавычек
                for (int k=0; k<7; k++){ //пробегаемся по массиву метасимволов (проверка метасимвол или нет)
                    if(*c == metaArr[k]){
                        if(word != NULL){
                            if(indexStr == sizeArr-1){ //отслеживаем, чтобы хватало на NULL и слово, и метаслово
                                sizeArr += SIZE_ARR;
                                arr = (char **) realloc(arr, sizeArr * sizeof(char *));
                            }
                            word[indexW] = 0;
                            arr[indexStr] = word;
                            indexStr++;
                            arr[indexStr] = NULL;
                            word = NULL; //возвращаем в начальное состояние слово
                            indexW = 0;
                            sizeWord = 0;  
                        }
                        sizeMetaWord = 2;
                        metaWord = (char *) malloc(sizeMetaWord * sizeof(char)); //создаем метаслово
                        metaWord[0] = *c;
                        if((*c == '>')||(*c == '|')||(*c == '&')){
                            wasMeta = 1;
                            mc = getc(i);
                            if(*c == mc){
                                sizeMetaWord++;
                                metaWord = (char*) realloc(metaWord, sizeMetaWord * sizeof(char));
                                metaWord[1] = *c;
                                metaWord[2] = '\0';
                            } else metaWord[1] = '\0';
                        }else metaWord[1] = '\0';
                        //printf("Это метаслово ---> %s\n", metaWord);
                        //в этот момент слово уже создано
                        if(indexStr == sizeArr-1){ //отслеживаем, чтобы хватало на NULL и слово, и метаслово
                            sizeArr += SIZE_ARR;
                            arr = (char **) realloc(arr, sizeArr * sizeof(char *));
                        }
                        arr[indexStr] = metaWord;
                        indexStr++;
                        arr[indexStr] = NULL;
                        break;
                    }
                }
            }
            if(metaWord == NULL){ //выполняется, если с не метасимвол и так далее (то есть обычный, либо в режиме кавычек)
                if(sizeWord == 0){ //создаем слово
                    //printf("Я создал слово\n");
                    word = (char *) malloc(SIZE_WORD * sizeof(char));
                    sizeWord = SIZE_WORD;
                }
                else 
                    if(indexW == sizeWord-1){ //увеличиваем размер слова, если не хватит, чтобы вместить символ
                        sizeWord += SIZE_WORD;
                        word = (char *) realloc(word, sizeWord * sizeof(char));
                    }
                word[indexW] = *c;
                indexW++;
                *c = getc(i);
            }
            else{
                if (wasMeta == 1){
                    wasMeta = 0;
                    *c = mc;
                }
                else *c = getc(i); 
                metaWord = NULL; //возвращаем в начальное состояние метаслово
                sizeMetaWord = 0;
            }
        }
    }
    //printf("Я снаружи\n");
    if(cQoute1+cQoute2>0){
         *errorStr += 1;
         if(word != NULL) free(word);
         freeStr(arr);
         free(arr);
         return NULL;
    }
    if((*c == '\n')||(*c == EOF)){
        if(word != NULL){ //если не равен нулю, то добавляем слово
            if(indexStr == sizeArr-1){ //отслеживаем, чтобы хватало на NULL и слово
                sizeArr += SIZE_ARR;
                arr = (char **) realloc(arr, sizeArr * sizeof(char *));
            }
            //if (*arr == NULL) printf("ну ты лох\n");
            word[indexW] = '\0';
            arr[indexStr] = word;
            indexStr++;
            arr[indexStr] = NULL; 
        }
        return arr;
    }
    freeStr(arr);
    free(arr);
    return NULL;
}


//Функция освобождает память (в конце массиво NULL, это обязательно)
void freeStr(char ** arrWords){
    while(*arrWords != NULL){
        free(*arrWords);
        arrWords++;
    }
}
void printArr(char ** arrWords){
    while(*arrWords != NULL){
                printf("%s\n", *arrWords);
                arrWords++;
    }
}