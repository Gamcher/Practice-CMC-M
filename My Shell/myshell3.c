#define _GNU_SOURCE
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

#define SIZE_ARR 5
#define SIZE_WORD 5
#define COLOR "\e[0;36m"
#define RESET "\e[m"

char * metaArr = "|><;&()"; //массив метасимволов
volatile int pCheck;
char ** readStr(int *, int *, FILE *);
void freeStr(char **);
void printArr(char **);
int findStr(char *, char **);
void deleteNWords(int, char **, int);
char ** dupNWords(int, char **, int);
void handlerChild();
void conveyor(char**, int, int, int *);
int sizeArray(char **);
void handlerPhoneKill();
void executeCommand(char **, int *, int *);

int main(int argc, char ** argv){
    //работа с файлом для ввода
    FILE *i;
    if (argc > 1)   
        i = fopen(argv[1], "r");
    else  i = stdin;
    //работа со строками
    char * pathname = get_current_dir_name();
    printf("%s%s>%s",COLOR, pathname, RESET);
    free(pathname);
    int c = getc(i);
    pCheck = 0;
    while(c != EOF){
        int errorStr = 0; //ошибки помогают понять, есть ли проблемы с кавычками
        char ** arWords = readStr(&c, &errorStr, i); //считывание построчно
        if(arWords != NULL){
            if(arWords[0] != NULL){
                int status, j = 0;
                int checkSemicolon;
                do{ 
                    char ** commandArray = NULL; 
                    if (!strcmp(arWords[j],";")){   
                        deleteNWords(j, arWords, 1);
                    }
                    checkSemicolon = findStr(";", arWords);
                    if (checkSemicolon < 0){
                        commandArray = dupNWords(j, arWords, sizeArray(arWords) - j);
                        j += sizeArray(arWords) - j;
                    }
                    else {
                        commandArray = dupNWords(j, arWords, checkSemicolon-j);
                        j += checkSemicolon-j;
                    }
                    executeCommand(commandArray, &status, &j);
                    free(commandArray);
                } while(arWords[j] != NULL);
                freeStr(arWords);//функция освобождения памяти
            }
            free(arWords);  
        }else
            if(errorStr != 0) fprintf(stderr, "Есть неправильное количество кавычек\n");
        if(c == EOF) break;
        else{
            pathname = get_current_dir_name();
            printf("%s%s>%s",COLOR, pathname, RESET);
            free(pathname);
            c = getc(i);
        }
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
                                wasMeta++;
                                sizeMetaWord++;
                                metaWord = (char*) realloc(metaWord, sizeMetaWord * sizeof(char));
                                metaWord[1] = *c;
                                metaWord[2] = '\0';
                            } else metaWord[1] = '\0';
                        }else metaWord[1] = '\0';
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
                fprintf(stdout, "%s\n", *arrWords);
                arrWords++;
    }
}

int findStr(char * str, char ** arrWords){
    for(int i = 0; arrWords[i] != NULL; i++)
        if(strcmp(arrWords[i], str) == 0) return i;
    return -1;
}

void deleteNWords(int index, char ** arrWords, int n){
    int i;
    for(i = 0; (i<n)&&(arrWords[index] != NULL); i++) free(arrWords[index+i]); 
    int j;
    for(j = index; arrWords[index+i] != NULL; i++, j++){
        arrWords[j] = arrWords[index+i];
    }
    arrWords[j] = arrWords[index+i];
}

char ** dupNWords(int index, char ** arrWords, int n){
    char ** newArrWords = (char **) malloc((n+1) * sizeof(char *));
    int i = 0;
    for(; i < n; i++) newArrWords[i] = arrWords[index+i];
    newArrWords[i] = NULL;
    return newArrWords;
}

void handlerChild(){
    if(pCheck > 0){
    int status;
    int pid = wait(&status);
        if(WIFEXITED(status) > 0) printf("Process %d exited with code %d\n", pid, WEXITSTATUS(status));
        else printf("Process %d aborted by signal %d\n", pid, WTERMSIG(status));
        pCheck--;
        if(pCheck == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
        }
    }
}

void conveyor(char** arrWords, int fdo1, int fdo2, int * status)
{
    pid_t pid;
    char ** commandArray;
    int fd[2];
    int i = 0;
    int convCheck;
    while (arrWords[i] != NULL) 
    {   
        if (!strcmp(arrWords[i],"|"))
        {   
            deleteNWords(i, arrWords, 1);
            continue;
        }
        convCheck = findStr("|", arrWords);
        if (convCheck < 0){
            commandArray = dupNWords(i, arrWords, sizeArray(arrWords) - i);
            i += sizeArray(arrWords) - i;
        }
        else {
            commandArray = dupNWords(i, arrWords, convCheck-i);
            i += convCheck-i;
        }
        pipe(fd);
        pid = fork();
        if (pid == -1)
        {
            perror("System error with pid");
            exit(*status);
        }
        else if (pid == 0)
        {
            if (i != sizeArray(arrWords)) dup2(fd[1], 1);
            else{
                if (fdo1 >= 0){
                    dup2(fdo1, 1);
                    close(fdo1);
                }
                if (fdo2 >= 0){
                    dup2(fdo2, 1);
                    close(fdo2);
                }
            }
            close(fd[1]);
            close(fd[0]);
            if (execvp(commandArray[0], commandArray) == -1){
                fprintf(stderr, "Такой функции нет. Код ошибки - %d\n", errno);
                freeStr(arrWords);
                free(arrWords);
                exit(*status);
            }
        }
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        free(commandArray);
    }
    while (wait(status) != -1);
}

int sizeArray(char ** arrWords){
    int i = 0;
    while(arrWords[i] != NULL) i++;
    return i;
}
void handlerPhoneKill(){
    kill(0, SIGKILL);
    signal(SIGCHLD, SIG_DFL);
}

void executeCommand(char ** arrWords, int * status, int * i){
    if(strcmp(arrWords[0], "cd") != 0){
        int pid;
        int check;
        int convCheck = findStr("|", arrWords); // проверка, что конвейер
        if (convCheck>= 0){ // это конвейер
            if((pid =fork()) > 0){
                signal(SIGCHLD, SIG_DFL);
                waitpid(pid, status, 0);
            } 
            else if(pid == 0){
                int fdo1, fdo2;
                fdo1 = fdo2 = -1;
                int check = findStr("<", arrWords);
                if(check >= 0){ //для перенаправления ввода
                    int fdi = open(arrWords[check+1], O_RDONLY);
                    if (fdi < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления ввода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                    dup2(fdi, 0);
                    close(fdi);
                }
                check = findStr(">", arrWords); //для перенаправления вывода
                if(check >= 0){
                    fdo1 = open(arrWords[check+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                    if (fdo1 < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления вывода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                }
                check = findStr(">>", arrWords); //для перенаправления вывода с дописыванием
                if(check >= 0){
                    fdo2 = open(arrWords[check+1], O_WRONLY | O_CREAT | O_APPEND, 0600);
                    if (fdo2 < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления вывода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                }
                conveyor(arrWords, fdo1, fdo2, status);

                exit(*status);
            } else fprintf(stderr, "Не хватает ресурсов или превышен лимит процессов\n");
        }
        else{
            int phoneCheck = findStr("&", arrWords); //проверка и удаление символа фонового режима
            if(phoneCheck >= 0){
                    deleteNWords(phoneCheck, arrWords, 1);
                    pCheck++;
                    signal(SIGINT, SIG_IGN);
                    signal(SIGQUIT, SIG_IGN);
            }
            if((pid = fork()) > 0){
                signal(SIGCHLD, handlerChild);
                if(phoneCheck < 0){
                    signal(SIGCHLD, SIG_DFL);
                    waitpid(pid, status, 0);
                    signal(SIGCHLD, handlerChild);
                }
            }
            else if (pid == 0){
                if(phoneCheck >= 0){ //перенаправление вывода на /dev/null
                    int fd1 = open("/dev/null", O_RDONLY);
                    dup2(fd1, 0);
                    close(fd1);
                    // int fd2 = open("/dev/null", O_WRONLY);
                    // dup2(fd2, 1);
                    // close(fd2);
                    signal(SIGINT, SIG_IGN);
                }
                check = findStr("<", arrWords);
                if(check >= 0){ //для перенаправления ввода
                    int fd = open(arrWords[check+1], O_RDONLY);
                    if (fd < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления ввода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                    dup2(fd, 0);
                    close(fd);
                }
                check = findStr(">", arrWords); //для перенаправления вывода
                if(check >= 0){
                    int fd = open(arrWords[check+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                    if (fd < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления вывода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                    dup2(fd, 1);
                    close(fd);
                }
                
                check = findStr(">>", arrWords); //для перенаправления вывода с дописыванием
                if(check >= 0){
                    int fd = open(arrWords[check+1], O_WRONLY | O_CREAT | O_APPEND, 0600);
                    if (fd < 0){
                        fprintf(stderr, "Некорректное имя файла для перенаправления вывода\n");
                        _exit(*status);
                    }
                    deleteNWords(check, arrWords, 2);
                    dup2(fd, 1);
                    close(fd);
                }

                if (execvp(arrWords[0], arrWords) == -1){
                    fprintf(stderr, "Такой функции нет. Код ошибки - %d\n", errno);
                    freeStr(arrWords);
                    free(arrWords);
                    exit(*status);
                }
            }
            else fprintf(stderr, "Не хватает ресурсов или превышен лимит процессов\n");
        }
    } else{
        if (arrWords[1] != NULL){
            if(arrWords[2] != NULL)
                fprintf(stderr, "Слишком много аргументов\n");
            else
                if (chdir(arrWords[1]) == -1)
                    fprintf(stderr, "Такого каталога нет, код ошибки - %d\n", errno);
        } else chdir(getenv("HOME"));
    }
}