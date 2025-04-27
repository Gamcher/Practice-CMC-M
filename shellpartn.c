#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>

#define SIZE 10

int special[] = {'&', '|', ';', '>', '<', '(', ')'};
int doubles[] = {'&', '|', '>'};
int errors = 0;
pid_t global_pid;

int array_check(int c, int* m, int l);
int array_check_strings(char* s, char** m, int l);
int array_length(char** m);
void array_free(char** m, int l);
void array_delete(int pos, char** m, int l);
char** array_create(int pos, char*** m, int l);
int process(char*** copy, char*** arguments, int larg, int lcopy);
void background_process(char*** arguments, char*** copy, int larg, int lcopy);
int conveyor(char*** arguments, char*** copy, int larg, int lcopy);
int chunk(char*** arguments, int larg);
char* getword(FILE* in);
void IntHndlr(int status);
//void ChldHndlr(int status);
//void BgHndlr(int status);
void printArr(char ** arrWords){
    while(*arrWords != NULL){
                fprintf(stdout, "%s\n", *arrWords);
                arrWords++;
    }
}

int array_check(int c, int* m, int l){
    for (int i = 0; i < l; i++){
        if (m[i] == c) return 1;
    }
    return 0;
}

int array_check_strings(char* s, char** m, int l){
    int strl = strlen(s);
    for (int i = 0; i < l; i++){
        if (!strncmp(m[i], s, strl)) return i;
    }
    return -1;
}

void array_free(char** m, int l){
    for (int i = 0; i < l; i++) free(m[i]);
    free(m);
}

int array_length(char** m){
    int i = 0;
    while(m[i] != NULL) i++;
    return i;
}

void array_delete(int pos, char** m, int l){
    int i, j;
    for (i = 0; (i < l) && (m[pos] != NULL); i++) free(m[pos + i]);
    for (j = pos; m[pos + i] != NULL; i++, j++){
        m[j] = m[pos + i];
    }
    m[j] = m[pos + i];
}

char** array_create(int pos, char*** m, int l){
    int i;
    char** t  = (char**)malloc((l+2)*sizeof(char*));
    for (i = 0; i < l; i++) t[i] = strdup((*m)[pos+i]);
    t[i] = NULL;
    return t;
}

void manual_cd(char** m){
    char* loc = getenv("HOME");
    if (m[1] != NULL){
        loc = malloc(_PC_PATH_MAX+1);
        char* prevloc = malloc(_PC_PATH_MAX+1);
        getcwd(prevloc,_PC_PATH_MAX);
        sprintf(loc, "%s/%s", prevloc, m[1]);
        free(prevloc);
    }
    if (chdir(loc) == -1) perror("Invalid directory.\n");
    if (m[1] != NULL) free(loc);
    return;
}

void input_redirection(int fd){
    if (fd < 0){
        perror("Invalid input file, redirecting to /dev/null.\n");
        fd = open("/dev/null", O_RDONLY);
        dup2(fd, 0);
        close(fd);
    }
    else{
        dup2(fd, 0);
        close(fd);
    }
    return;
}

void output_redirection(int fd){
    if (fd < 0) perror("Invalid output file.\n");
    else{
        dup2(fd, 1);
        close(fd);
    }
    return;
}

void IntHndlr(int status){
    kill(global_pid, SIGINT);
}

/*void ChldHndlr(int status){
    pid_t pid = wait(&status);
    if (WIFEXITED(status) > 0) printf("Process %d exited with code %d.\n", pid, WEXITSTATUS(status));
    else printf("Process %d aborted by signal %d.\n", pid, WTERMSIG(status));
    return;
}*/

/*void BgHndlr(int status){
    kill(0, SIGKILL);
    signal(SIGCHLD, SIG_DFL);
}*/

int process(char*** arguments, char*** copy, int larg, int lcopy){
    char** m = *arguments;
    if (m[0] == NULL) return -1;
    int ofs = 0;
    for (int j = 0; j < larg; j++){
        if (!(strcmp(m[j], "<")) && (j < larg - 1)){
            int fd = open(m[j+1], O_RDONLY);
            input_redirection(fd);
            j++;
            ofs += 2;
        }
        else if (!(strcmp(m[j], ">")) && (j < larg - 1)){
            int fd = open(m[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            output_redirection(fd);
            j++;
            ofs += 2;
        }
        else if (!(strcmp(m[j], ">>")) && (j < larg - 1)){
            int fd = open(m[j+1], O_WRONLY | O_CREAT | O_APPEND, 0600);
            output_redirection(fd);
            j++;
            ofs += 2;
        }
    }
    if(!(strcmp(m[0], "("))){
        int sus = 1;
        while(strcmp(m[sus], ")") && (m[sus] != NULL)) sus++;
        if (m[sus] == NULL){
            perror("Brackets don't match.\n");
            array_free(*arguments, larg);
            array_free(*copy, lcopy);
            exit(1);
        }
        else{
            int status;
            sus--;
            char** secondary = array_create(1, arguments, sus);
            if(fork() == 0){
                array_free(*arguments, larg);
                array_free(*copy, lcopy);
                status = chunk(&secondary, sus);
                exit(WEXITSTATUS(status));
            }
            wait(&status);
            array_free(secondary, sus);
            return status;
        }
    }
    if (!strcmp(m[0], "cd")){
        manual_cd(m);
        return 0;
    }
    int sus = larg - ofs;
    char** secondary = (char**)malloc((sus+2)*sizeof(char*));
    for(int j = 0; j < sus; j++){
        if(!(strcmp(m[j], "<")) || !(strcmp(m[j], ">")) || !(strcmp(m[j], ">>"))) j++;
        else secondary[j] = strdup(m[j]);
    }
    secondary[sus] = NULL;
    int status;
    if (fork() == 0){
        execvp(secondary[0], secondary);
        if (status == -1) perror("Invalid command.\n");
        array_free(*arguments, larg);
        array_free(*copy, lcopy);
        array_free(secondary, sus);
        exit(1);
    }
    wait(&status);
    array_free(secondary, sus);
    return status;
}

void background_process(char*** copy, char*** arguments, int lcopy, int larg){
    if (fork() == 0){
        int status, in = open("/dev/null", O_RDONLY);
        if (in == -1) return;
        dup2(in, 0);
        signal(SIGINT,SIG_IGN);
        if (fork() == 0){
            status = conveyor(copy, arguments, lcopy, larg);
            if(WIFSIGNALED(status)) printf("Process PID aborted by signal %d\n", WTERMSIG(status));
            else printf("Process PID exited with code %d\n", WEXITSTATUS(status));
            array_free(*arguments, larg);
            array_free(*copy, lcopy);
            exit(0);
        }
        array_free(*arguments, larg);
        array_free(*copy, lcopy);
        close(in);
        exit(0);
    }
}

int conveyor(char*** arguments, char*** copy, int larg, int lcopy){
    char** m = *arguments;
    int status = 0, sus, t, c = 0, ofs = 0, fd[2];
    for(t = 0; t < larg; t++){
        if (!strcmp(m[t], "|")) c++;
    }
    c++;
    for (t = 0; t < c; t++){
        sus = 0;
        while ((m[ofs+sus] != NULL) && strcmp(m[ofs+sus], "|") && strcmp(m[ofs+sus], ")")) sus++;
        char** secondary = array_create(ofs, &m, sus);
        ofs += (sus + 1);
        pipe(fd);
        if(fork() == 0){
            if ((t + 1) != c) dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            //array_free(*copy, lcopy);
            status = process(&secondary, arguments, sus, larg);
            array_free(secondary, sus);
            array_free(*arguments, larg);
            exit((WEXITSTATUS(status)));
        }
        //array_free(secondary, sus);
        free(secondary);
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
    }
    while(wait(&status) != -1);
    return status;
}

int chunk(char*** arguments, int larg){
    char** m = *arguments;
    int status = 0, ofs = 0;
    for (int j = 0; j < larg; j++){
        if (!(strcmp(m[j], "("))){
            while(strcmp(m[j], ")") && (m[j] != NULL)) j++;
            if(m[j] == NULL){
                perror("Brackets don't match.\n");
                array_free(*arguments, larg);
                exit(1);
            }
        }
        else if (!(strcmp(m[j], ";"))){
            if((!j) || (j == larg - 1)){
                perror("Invalid use of semicolon.\n");
                array_free(*arguments, larg);
                exit(1);
            }
            int sus = j - ofs;
            char** secondary = array_create(ofs, &m, sus);
            if (fork() == 0){
                status = conveyor(&secondary, arguments, sus, larg);
                array_free(secondary, sus);
                array_free(*arguments, larg);
                exit(WEXITSTATUS(status));
            }
            wait(&status);
            array_free(secondary, sus);
            ofs = j + 1;
        }
        else if (!(strcmp(m[j], "&&"))){
            if((!j) || (j == larg - 1)){
                perror("Invalid use of double ampersand.\n");
                array_free(*arguments, larg);
                return -1;
            }
            int sus = j - ofs;
            char** secondary = array_create(ofs, &m, sus);
            if (fork() == 0){
                status = conveyor(&secondary, arguments, sus, larg);
                array_free(secondary, sus);
                array_free(*arguments, larg);
                exit(WEXITSTATUS(status));
            }
            wait(&status);
            if ((WEXITSTATUS(status))){
                j++;
                while(strcmp(m[j], "&&") && strcmp(m[j], ";") && strcmp(m[j], "&") && strcmp(m[j], "||") && (j < larg-1)) j++;
            }
            array_free(secondary, sus);
            ofs = j + 1;
        }
        else if (!(strcmp(m[j], "||"))){
            if ((!j) || (j == larg - 1)){
                perror("Invalid use of double vertical bar.\n");
                array_free(*arguments, larg);
                return -1;
            }
            int sus = j - ofs;
            char** secondary = array_create(ofs, &m, sus);
            if (fork() == 0){
                status = conveyor(&secondary, arguments, sus, larg);
                array_free(secondary, sus);
                array_free(*arguments, larg);
                exit(WEXITSTATUS(status));
            }
            wait(&status);
            if (!(WEXITSTATUS(status))){
                j++;
                while(strcmp(m[j], "&&") && strcmp(m[j], ";") && strcmp(m[j], "&") && strcmp(m[j], "||") && (j < larg-1)) j++;
            }
            array_free(secondary, sus);
            ofs = j + 1;
        }
        else if (!(strcmp(m[j], "&"))){
            int sus = j - ofs;
            char** secondary = array_create(ofs, &m, sus);
            background_process(&secondary, arguments, sus, larg);
            array_free(secondary, sus);
            ofs = j + 1;
        }
        else if (!(strcmp(m[j], "cd"))){
            j++;
            char* loc = getenv("HOME");
            if (m[j] != NULL){
                loc = malloc(_PC_PATH_MAX+1);
                char* prevloc = malloc(_PC_PATH_MAX+1);
                getcwd(prevloc,_PC_PATH_MAX);
                sprintf(loc, "%s/%s", prevloc, m[j]);
                free(prevloc);
            }
            if (chdir(loc) == -1) perror("Invalid directory.\n");
            if (m[j] != NULL) free(loc);
            ofs = j + 1;
        }
    }
    signal(SIGINT, IntHndlr);
    int sus = larg - ofs;
    char** secondary = array_create(ofs, &m, sus);
    if ((global_pid = fork()) == 0){
        signal(SIGINT, SIG_DFL);
        if (secondary[0] != NULL){
            status = conveyor(&secondary, arguments, sus, larg);
        }
        array_free(secondary, sus);
        array_free(*arguments, larg);
        exit(WEXITSTATUS(status));
    }
    wait(&status);
    signal(SIGINT, SIG_DFL);
    array_free(secondary, sus);
    array_free(*arguments, larg);
    return status;
}

char* getword(FILE* in){
    int c = 0, k = 0, n = SIZE, q = 0;
    c = getc(in);
    if (c == EOF) return NULL;
    if (c == '\n') return "\n";
    if (c == ' ') return " ";
    char* word = malloc(SIZE);
    if (word == NULL) return NULL;
    if (array_check(c, special, 7)){
        word[0] = c;
        if (array_check(c, doubles, 4)){
            c = getc(in);
            if (c == word[0]){
                word[1] = (char)c;
                word[2] = '\0';
            }
            else{
                ungetc(c, in);
                word[1] = '\0';
            }
        }
        else word[1] = '\0';
        return word;
    }
    while (c != '\n'){
        if (c == '"'){
            q++;
            q %= 2;
            c = getc(in);
            continue;
        }
        if((isspace(c) || array_check(c, special, 7)) && !q){
            ungetc(c, in);
            word[k] = '\0';
            return word;
        }
        if(k == n-2){
            n *= 2;
            word = realloc(word, n);
            if (!word) return NULL;
        }
        word[k] = (char)c;
        k++;
        c = getc(in);
        if(c == EOF){
            ungetc(c, in);
            word[k] = '\0';
            return word;
        }
    }
    if (q == 1){
        perror("Quotes don't match\n");
        free(word);
        errors = 1;
        return "\n";
    }
    word[k] = '\0';
    ungetc(c, in);
    return word;
}

int main(int argc, char** argv){
    FILE* in = stdin;
    if (argc > 1) in = fopen(argv[1], "r");
    char* word = NULL;
    while((word = getword(in)) != NULL){
        int i = 0, n = SIZE;
        char** word_array = malloc(sizeof(char*) * SIZE);
        while(word && strcmp(word, "\n")){
            if(strncmp(word, " ", 1)){
                if (i == n-1){
                    n++;
                    word_array = realloc(word_array, sizeof(char*)*n);
                }
                word_array[i] = word;
                word_array[i+1] = NULL;
                i++;
            }
            word = getword(in);
        }
        if ((!errors) && (i > 0)) chunk(&word_array, i);
        if (i == 0) free(word_array);
        errors = 0;
        if (word == NULL) break;
        wait(NULL);
    }
    free(word);
    if (argc > 1) fclose(in);
}