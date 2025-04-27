#include "matrix_my.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** in_matrix(int m, int n){
    printf("%s\n","Пиши матрицу:" );
    double** matr = (double**)malloc(m * sizeof(double*));
    for (int i=0; i<m; i++){
        matr[i] = (double*)calloc(n, sizeof(double));
        for (int j=0; j<n; j++)
            if (scanf("%lf ", &(matr[i][j]))==0){
                free_matr(matr, i);
                return NULL;
            }
    }
    return matr;
}

void out_matr(double** matr, int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++)
            printf("%lf ", matr[i][j]);
        printf("\n");
    }
}

double** add_matr(double** matr1, int m1, int n1, double** matr2, int m2, int n2){
    if ((m1 == m2) && (n1 == n2)) {
        double** matr = (double**)malloc(m1 * sizeof(double*));
        for (int i=0; i<m1; i++)
            matr[i] = (double*)calloc(n1, sizeof(double));
        for(int i=0; i<m1; i++)
            for(int j=0; j<n1; j++)
                matr[i][j] = matr1[i][j] + matr2[i][j];
        return matr;       
    }
    else{
        printf("%s\n", "Не выполнены правила сложения");
        return NULL;
    }
}

void muln_matr(double a, double** matr, int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            matr[i][j] = a * matr[i][j];
        }
    }
}
double** mul_matr(double** matr1, int m1, int n1, double** matr2, int m2, int n2){
    if (n1 == m2){
        int summa;
        double** matr = (double**)malloc(m1 * sizeof(double*));
        for (int i=0; i<m1; i++)
            matr[i] = (double*)calloc(n2, sizeof(double));
        for(int i=0; i<m1; i++){
            for(int j=0; j<n2; j++){
                summa = 0;
                for(int k=0; k<n1; k++){
                    summa += matr1[i][k] * matr2[k][j];
                }
                matr[i][j] = summa;
            }
        }
        return matr;
    }
    else{
        printf("%s\n", "Не выполнены правила умножения");
        return NULL;
    }
}
double det_matr(double** matr, int n    ){
    if (!matr){
        printf("Нет\n");
        return 0;
    }
    if (n<=0) {
        printf("Ноу\n");
        return 0;
    }
    if (n==1){
        return **matr;
    }
    int i,j,k;
    double det=0;
    int h = -1;
    for (k=0;k<n;k++){
        h = -h;
        double** nmat = calloc(n-1,sizeof(double*));
        for (i=0;i<n-1;i++){
            nmat[i]=calloc(n-1,sizeof(double));
            for (j=0;j<n;j++){
                if (j<k){
                    nmat[i][j]=matr[i+1][j];
                }
                if (j>k){
                    nmat[i][j-1]=matr[i+1][j];
                }
            }
        }
        det=det + det_matr(nmat,n-1)*(matr[0][k])*h;
        free_matr(nmat,n-1);
    }
    return det;    
}

void free_matr(double** matr, int m){
    for(int i=0; i<m; i++)
        free(matr[i]);
    free(matr);
}