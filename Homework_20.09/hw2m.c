#include <stdio.h>
#include <string.h>

void reverse(char *);
void cdelete(char *, char);
int mystrcmp(char *, char *);

int main(){
	char str1[]= "abfdgte";
	char str2[]= "abfdgte";
	printf("%d\n", mystrcmp(str1, str2));
	return 0;
}

void reverse(char *s){
	char c;
	char *sti, *stj;
	for(sti = s,stj=s+strlen(s)-1;stj-sti>0;sti++,stj--){
		c = *sti;
		*sti = *stj;
		*stj = c;
	}
}
void cdelete(char *s, char c){
	char *dst = s;
	while(*dst){
		if(*dst != c){
			*s=*dst;
			s++;
		}	
		dst++;
	}
	*s=0;
}
int mystrcmp(char *s1, char *s2){
	while((*s1 == *s2)&&(*s1)){
		s1++; s2++;
	}
	return *s1-*s2;
}

