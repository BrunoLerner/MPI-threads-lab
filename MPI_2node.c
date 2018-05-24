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

int total_proc(){
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    return p;
}

int **matrix;

void getMatrix() {
    // Alocando espaço pra matriz
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
}

int * normalizeMatrix(int **matrix) {
    int *matrixArray;
    for(int i = 0; i < size*size; i++) {
        matrixArray[i] = matrix[i/size][i%size];
    }
    return matrixArray;
}

int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    if(proc_index() == 0) {
        // O objetivo desse nó é pegar a matriz e distribuir pro resto
        getMatrix();
        int *matrixArray = normalizeMatrix(&matrix);
        MPI_Send(&matrixArray, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if(proc_index() == 1) {
        matrixArray = (int*) malloc( SIZE/2*sizeof(int));
		
		for(i = 0; i < SIZE/2; i++) {
			for(j = 0; j < SIZE; j++) {
				MPI_Recv(&matrixArray, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
    }

    MPI_Finalize();
    return 0;
}
 
