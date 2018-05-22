#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define size 16000

int main(){

    // Alocando espaço pra matriz
    int **matrixMalloc, i, j, matrix[size][size];
    matrixMalloc = malloc(size * sizeof(int *));
    
    if(matrixMalloc == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for(int i = 0; i < size; i++) {
        matrixMalloc[i] = malloc(size * sizeof(int));
        if(matrixMalloc[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    
    FILE *matrixFile = fopen("matrix.txt","r");
    if(matrixFile == NULL) {
        printf("file could not be opened");
        exit(1);
    }

    i=0;
    while(!feof(matrixFile)) {
        char temp[size], *token;
        fscanf(matrixFile, "%s", temp);
        token = strtok(temp, ",");
        j=0;
        while(token != NULL) {
            matrix[i][j] = atoi(token);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }


    return 0;
}
 
