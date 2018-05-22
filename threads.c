#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define size 16000

int main(){

    // Alocando espaço pra matriz
    int **matrixFile, i, j, matrix[size][size];
    matrixFile = malloc(size * sizeof(int *));
    
    if(matrixFile == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for(int i = 0; i < size; i++) {
        matrixFile[i] = malloc(size * sizeof(int));
        if(matrixFile[i] == NULL) {
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
            matrix[i][j] = atoi(temp);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }


    return 0;
}
 
