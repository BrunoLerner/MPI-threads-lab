#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include "mpi.h"
#define size 16000


int proc_index(){
    int index;
    MPI_Comm_rank(MPI_COMM_WORLD, &index);
    return index;
}

int ** getMatrix() {
    // Alocando espaço pra matriz
    int **matrix;
    matrix = malloc(size * sizeof(int *));
    
    if(matrix == NULL) {
        printf("Out of memory\n");
        exit(1);
    }

    for(int i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
        if(matrix[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    // Gerando a matriz aleatoriamente
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
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
    int *matrixArray;
    for(int i = 0; i < size*size; i++) {
        matrixArray[i] = matrix[i/size][i%size];
    }
    return matrixArray;
}

int ** arrayToMatrix(int **matrixArray) {
    int **matrix;
    for(int i = 0; i < size*size; i++) {
        matrix[i/size][i%size] = matrixArray[i];
    }
    return matrix;
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int localSum = 0, globalSum;

    if(proc_index() == 0) {
        // O objetivo desse nó é pegar a matriz e distribuir pro resto
        int **matrix = getMatrix();
        int *matrixArray = matrixToArray(&matrix);
        MPI_Send(&matrixArray, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        int columns = size/2, rows = size, start = 0;
        printf("Esse é o nó %d calculando metade da matriz", proc_index());
        for(int i = 0; i < rows; i++) {
            for(int j = start; j < start + columns; j++) {
                matrix[i][j] *= matrix[i][i];
            }
        }

        for(int i = 0; i < rows; i++) {
            for(int j = start; j < start + columns; j++) {
                localSum += matrix[i][j];
            }
        }           
    }
    else if(proc_index() == 1) {
        matrixArray = (int*) malloc((SIZE/2)*sizeof(int));
        MPI_Recv(&matrixArray, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int **matrix = arrayToMatrix(&matrixArray);
        int columns = size/2, rows = size, start = size/2;

        for(int i = 0; i < rows; i++) {
            for(int j = start; j < start + columns; j++) {
                matrix[i][j] *= matrix[i][i];
            }
        }

        for(int i = 0; i < rows; i++) {
            for(int j = start; j < start + columns; j++) {
                localSum += matrix[i][j];
            }
        }  
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
             MPI_COMM_WORLD);

    // Print the result
    if (proc_index() == 0) {
        printf("A soma dos elementos da matriz resultante é = %d\n", global_sum));
    }

    
    MPI_Finalize();
    return 0;
}
 
