#include "matrix_my.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m = 2;
int n = 2;
int main(){
    double** mmatr;
    if((mmatr = in_matrix(m, n)) != NULL){                     
        printf("%lf\n", det_matr(mmatr, m)); 
        free_matr(mmatr, m); 
    }
    return 0;
}

