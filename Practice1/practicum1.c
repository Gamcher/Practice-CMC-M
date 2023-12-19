#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define M 10
#define SC sizeof(char*)

char *input_str(void);
void sortb_str(char**, int);
void out_arr_str(char**);
void free_arr_str(char**);

int main(){
    char **s = calloc(M, SC);
    char *c = input_str();
    int i=0;
    int k=M;
    if(c!= NULL){
        while(c!=NULL){
            if(i>=k-1){
                k+=M;
                s = realloc(s, k*SC);
            }
            s[i]= c;
            i++;
            c = input_str();
        }
        s[i]=NULL;
        sortb_str(s, i);
        printf("\n");
        printf("%s\n", "Отсортированные строки:");
        out_arr_str(s);
    }
    
    free_arr_str(s);
    free(s);
}

char *input_str(void){						
	int n, k=0;							 
	char *s = malloc(M);						 
	if (!s) return NULL;						 
	while (fgets (s+k, M, stdin)) {
		n = strlen (s);
		if (s[n-1] != '\n') {			
			k = k+M-1;				  	 
			s = realloc (s, k+M);			
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
void sortb_str(char **str, int n){
    int k;
    char *sstr;
	while(n>1) { 
		k=0; 
		for (int i=1; i<n;++i) {
			if(strcmp(str[i-1],str[i]) > 0) {
				sstr = str[i];
                str[i] = str[i-1];
                str[i-1] = sstr;
				k=i; 
			}
		}
		n=k; 
	}
     
}
void out_arr_str(char **str){
    while(*str != NULL){
        printf("%s\n", *str);
        str++;
    }   
}
void free_arr_str(char **arr){
    while(*arr != NULL){
        free(*arr);
        arr++;
    } 
}