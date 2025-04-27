#include <stdio.h>

int main(){
	int z = 100;
	z = (z & 31)|7;
	printf("%d\n", z);
	return 0;
}
