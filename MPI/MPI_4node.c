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
    
    if (matrix == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for (i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    // Gerando a matriz aleatoriamente
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            matrix[i][j] = rand() % 2;
        }
    }

    return matrix;
}

int * matrixToArray(int **matrix) {
    int *matrixArray, i;
    matrixArray = malloc(size*size* sizeof(int));
    for (i = 0; i < size*size; i++) {
        matrixArray[i] = matrix[i/size][i%size];
    }
    return matrixArray;
}

int ** arrayToMatrix(int *matrixArray) {
    int **matrix, i;
    matrix = malloc(size * sizeof(int *));
    
    if (matrix == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for (i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
        if(matrix[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }

    for (i = 0; i < size*size; i++) {
        matrix[i/size][i%size] = matrixArray[i];
    }
    return matrix;
}

int main(int argc, char** argv){
    
    struct timeval startTime, endTime;
    
    MPI_Init(&argc, &argv);

    int localSum = 0, globalSum, myRank, columns = size/2, rows = size/2;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        // O objetivo desse nó é pegar a matriz e distribuir pro resto
        int **matrix = getMatrix(), i, j;
        int *matrixArray = matrixToArray(matrix);
        int rowStart = 0, columnStart = 0;

        gettimeofday(&startTime, NULL);
        MPI_Send(&matrixArray[0], size * size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        for (i = rowStart; i < rowStart + rows; i++) {
            for (j = columnStart; j < columnStart + columns; j++) {
                matrix[i][j] *= matrix[i][i];
                localSum += matrix[i][j];
            }
        }

        printf("Nó %d diz: Estou enviando a matriz pros outros nós e o resultado parcial aqui deu %d\n", myRank, localSum);
    }
    else if(myRank == 1) {
        int *matrixArray = malloc(size * size * sizeof(int)), i, j;
        MPI_Recv(matrixArray, size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int **matrix = arrayToMatrix(matrixArray);
        int rowStart = 0, columnStart = size/2;
        int *diagonal = malloc(size * sizeof(int));
        for (i =0; i < size; i++) {
            diagonal[i] = matrix[i][i];
        }

        for (i = rowStart; i < rowStart + rows; i++) {
            for (j = columnStart; j < columnStart + columns; j++) {
                matrix[i][j] *= diagonal[i];
                localSum += matrix[i][j];
            }
        }

        printf("Nó %d diz: O resultado parcial aqui deu %d\n", myRank, localSum);
    }
    else if(myRank == 2) {
        int *matrixArray = malloc(size * size * sizeof(int)), i, j;
        MPI_Recv(matrixArray, size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int **matrix = arrayToMatrix(matrixArray);
        int rowStart = size/2, columnStart = 0;
        int *diagonal = malloc(size * sizeof(int));

        for (i = 0; i < size; i++) {
            diagonal[i] = matrix[i][i];
        }

        for (i = rowStart; i < rowStart + rows; i++) {
            for (j = columnStart; j < columnStart + columns; j++) {
                matrix[i][j] *= diagonal[i];
                localSum += matrix[i][j];
            }
        }

        printf("Nó %d diz: O resultado parcial aqui deu %d\n", myRank, localSum);

    }
    else if (myRank == 3) {
        int *matrixArray = malloc(size * size * sizeof(int)), i, j;
        MPI_Recv(matrixArray, size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int **matrix = arrayToMatrix(matrixArray);
        int rowStart = size/2, columnStart = size/2;

        for (i = rowStart; i < rowStart + rows; i++) {
            for (j = columnStart; j < columnStart + columns; j++) {
                matrix[i][j] *= matrix[i][i];
                localSum += matrix[i][j];
            }
        }

        printf("Nó %d diz: O resultado parcial aqui deu %d\n", myRank, localSum);
    }

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

    // Print the result
    if (myRank == 0) {
        gettimeofday(&endTime, NULL);
        double time = (endTime.tv_sec*1000000 + endTime.tv_usec) - (startTime.tv_sec*1000000 +  startTime.tv_usec);
        
        printf("A soma dos elementos da matriz resultante é = %d\n, tempo é = %d\n", global_sum, time/1000000.0));

    }

    MPI_Finalize();
    return 0;
}
 
