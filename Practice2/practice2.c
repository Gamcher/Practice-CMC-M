#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix_my.c"

int m = 2;
int n = 2;
int main(){
    double** mmatr = in_matrix(m, n);
    double  **m1, **m2;
    muln_matr(5, mmatr, m, n);
    out_matr(mmatr, m, n);
    m1 = mul_matr(mmatr, m, n, mmatr, m, n);
    out_matr(m1, m, n);
    m2 = add_matr(mmatr, m, n, mmatr, m, n);    
    out_matr(m2, m, n);                         
    printf("%lf\n", det_matr(mmatr, m)); 
    free_matr(mmatr, m); 
    free_matr(m1, m);
    free_matr(m2, m);
}
#include "matrix_my.h"
