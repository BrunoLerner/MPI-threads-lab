#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include "mpi.h"
#define size 16000

int ** getMatrix() {
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
    // fclose(matrixFile)
    return matrix;
}

int * matrixToArray(int **matrix) {
    int *matrixArray, i;
    matrixArray = malloc(size*size* sizeof(int));
    for(i = 0; i < size*size; i++) {
        matrixArray[i] = matrix[i/size][i%size];
    }
    return matrixArray;
}

int ** arrayToMatrix(int *matrixArray) {
    int **matrix, i;
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

    for(i = 0; i < size*size; i++) {
        matrix[i/size][i%size] = matrixArray[i];
    }
    return matrix;
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int localSum = 0, globalSum, myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if(myRank == 0) {
        // O objetivo desse nó é pegar a matriz e distribuir pro resto
        int **matrix = getMatrix(), i, j;
        int *matrixArray = matrixToArray(matrix);
        MPI_Send(&matrixArray[0], size * size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        int columns = size, rows = size/2, start = 0;
        for(i = start; i < start + rows; i++) {
            for(j = 0; j < columns; j++) {
                matrix[i][j] *= matrix[i][i];
                localSum += matrix[i][j];
            }
        }      
        printf("Nó %d diz: Aqui deu %d\n", myRank, localSum);
    }
    else if(myRank == 1) {
        int *matrixArray = malloc(size*size* sizeof(int)), i;
        MPI_Recv(matrixArray, size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("This is the matrix Im receiving\n");
        for (i = 0; i < 10; i++) {
            printf("%d ", matrixArray[i]);
        }
        printf("\n");
        int **matrix = arrayToMatrix(matrixArray), j;
        int columns = size, rows = size/2, start = size/2;

        for(i = start; i < start + rows; i++) {
            for(j = 0; j < columns; j++) {
                matrix[i][j] *= matrix[i][i];
                localSum += matrix[i][j];
            }
        }
        printf("Nó %d diz: Aqui deu %d\n", myRank, localSum);

    }

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

    // Print the result
    if (myRank == 0) {
        printf("A soma dos elementos da matriz resultante é = %d\n", globalSum);
    }
    
    MPI_Finalize();
    return 0;
}
 
