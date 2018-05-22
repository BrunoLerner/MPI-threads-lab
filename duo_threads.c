#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#define size 16000

int partialSum (int **matrix) {
    int index = 0;
    int columns = size/2, rows = size, sum=0;
    if(second_thread) {
        index = size/2;
    } 


    for(int i = 0; i < rows; i++) {
        for(int j = firstIndex; j < columns; j++) {
            matrix[i][j] *= matrix[i][i];
        }
    }
    for(int i = 0; i < rows; i++) {
        for(int j = firstIndex; j < columns; j++) {
            sum += matrix[i][j];
        }
    }
    
    return sum;
}

int main(){

    // Alocando espaço pra matriz
    int **matrix, i, j;
    matrix = malloc(size * sizeof(int *));
    
    if(matrix == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for(i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
        if(matrix[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    // Gerando a matriz aleatoriamente
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            matrix[i][j] = rand() % 2;
        }
    }
    
    // Lendo a matriz de um arquivo

    // FILE *matrixFile = fopen("matrix.txt","r");
    // if(matrixFile == NULL) {
    //     printf("file could not be opened");
    //     exit(1);
    // }

    // i=0;
    // while(!feof(matrixFile)) {
    //     char temp[size], *token;
    //     fscanf(matrixFile, "%s", temp);
    //     token = strtok(temp, ",");
    //     j=0;
    //     while(token != NULL) {
    //         matrix[i][j] = atoi(token);
    //         token = strtok(NULL, ",");
    //         j++;
    //     }
    //     i++;
    // }

    pthread_t first_thread, second_thread;
    pthread_create(&first_thread, NULL, partialSum, matrix);
    pthread_join(first_thread, NULL);
    printf("After Thread\n");


    return 0;
}
 
