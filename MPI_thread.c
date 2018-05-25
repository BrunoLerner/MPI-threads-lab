#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include "mpi.h"
#define size 16000
#define nThread 2

struct thread_args {
    int index;
    int procRank;
    int **matrix;
    int *diagonal;
};

int sum[2];

void * partialSum (void *matrixAndIndex) {
    struct thread_args * args = (struct thread_args *) matrixAndIndex;
    int index = args->index;
    int procRank = args->procRank;
    int **matrix = args->matrix;
    int *diagonal = args->diagonal;
    int columns = size/2, rows = size/2, rowStart = 0, columnStart = 0;
    printf("Esse é o thread %d do processador %d calculando uma metade da matriz.\n", index, procRank);

    if (procRank == 0 && index == 1) columnStart = size/2;
    else if (procRank == 1 && index == 0) rowStart = size/2;
    else if (procRank == 1 && index == 1) {
        rowStart = size/2;
        columnStart = size/2;
    }
    
    for (int i = rowStart; i < rowStart + rows; i++) {
        for (int j = columnStart; j < columnStart + columns; j++) {
            matrix[i][j] *= diagonal[i];
            sum[index] += matrix[i][j];
        }
    }
}

int ** getMatrix() {
    // Alocando espaço pra matriz
    int **matrix;
    matrix = malloc(size * sizeof(int *));
    
    if (matrix == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    // Gerando a matriz aleatoriamente
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 2;
        }
    }
    return matrix;
}

int * matrixToArray(int **matrix) {
    int *matrixArray;
    for (int i = 0; i < size*size; i++) {
        matrixArray[i] = matrix[i/size][i%size];
    }
    return matrixArray;
}

int ** arrayToMatrix(int **matrixArray) {
    int **matrix;
    for (int i = 0; i < size*size; i++) {
        matrix[i/size][i%size] = matrixArray[i];
    }
    return matrix;
}

int main(int argc, char** argv){
    
    struct timeval startTime, endTime;
    
    MPI_Init(&argc, &argv);

    int localSum = 0, globalSum, myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        // O objetivo desse nó é pegar a matriz e distribuir pro resto
        int **matrix = getMatrix();
        int *matrixArray = matrixToArray(&matrix);
        
        gettimeofday(&startTime, NULL);
        MPI_Send(&matrixArray, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Esse é o nó %d calculando metade da matriz", myRank);

        struct thread_args* matrixAndIndex;
        int *diagonal = malloc(size * sizeof(int));

        for (int i = 0; i < size ; i++) {
            diagonal[i] = matrix[i][i];
        }

        for (int i = 0; i < nThreads; i++) {
            matrixAndIndex = malloc(sizeof(struct thread_args));
            matrixAndIndex->index = i;
            matrixAndIndex->procRank = myRank;
            matrixAndIndex->matrix = matrix;
            matrixAndIndex->diagonal = diagonal;
            pthread_create(&threads[i], NULL, partialSum, (void *) matrixAndIndex);
        }
        
        for (int i = 0; i < nThreads; i++) {
            pthread_join(threads[i], NULL);
        }

        localSum = sum[0] + sum[1];
    }
    else if (myRank == 1) {
        matrixArray = (int*) malloc((SIZE/2)*sizeof(int));
        MPI_Recv(&matrixArray, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int **matrix = arrayToMatrix(&matrixArray);
        printf("Esse é o nó %d calculando metade da matriz", myRank);

        struct thread_args* matrixAndIndex;
        int *diagonal = malloc(size * sizeof(int));

        for (int i = 0; i < size ; i++) {
            diagonal[i] = matrix[i][i];
        }

        for (int i = 0; i < nThreads; i++) {
            matrixAndIndex = malloc(sizeof(struct thread_args));
            matrixAndIndex->index = i;
            matrixAndIndex->procRank = myRank;
            matrixAndIndex->matrix = matrix;
            matrixAndIndex->diagonal = diagonal;
            pthread_create(&threads[i], NULL, partialSum, (void *) matrixAndIndex);
        }
        
        for (int i = 0; i < nThreads; i++) {
            pthread_join(threads[i], NULL);
        }

        localSum = sum[0] + sum[1];
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

    // Print the result
    if (myRank == 0) {
        
        gettimeofday(&endTime, NULL);
        double time = (endTime.tv_sec*1000000 + endTime.tv_usec) - (startTime.tv_sec*1000000 +  startTime.tv_usec);
        
        printf("A soma dos elementos da matriz resultante é = %d\n, tempo é = %d", global_sum, time/1000000.0));
    }

    
    MPI_Finalize();
    return 0;
}
 
