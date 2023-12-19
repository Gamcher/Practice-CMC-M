#include <stdio.h>

void neg_arr(int **arr){
    int maxim = 0, k, ind_m;
    for (int i=0; i<50; i++){
        k = 0;
        for(int j=0; j<20; j++)
            if (arr[i][j]<0) k++;
        if (k > maxim){
            maxim = k;
            ind_m = i;
        }
    }
    for (int j=0; j<20; j++)
        printf("%d\t", arr[ind_m][j]);
}