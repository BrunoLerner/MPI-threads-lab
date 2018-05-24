#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>

#define size 16000
#define nThreads 4

typedef struct threads_args {
    int index;
    int * diagonal;
} pthread_args;

int sum[4], **matrix;

void * partialSum (void *matrixAndIndex) {
    pthread_args * args = (pthread_args *) matrixAndIndex;
    int index = args->index;
    int *diagonal = args->diagonal;
    int columns = size/4, rows = size/4, rowStart = 0, columnStart = 0;

    if(index == 1) {
        columnStart = size/4;
    } 
    else if (index == 2) {
        rowStart = size/4;
    }
    else if (index == 3) {
        rowStart = size/4;
        columnStart = size/4;
    }


    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            matrix[rowStart + i/rows][columnStart + j%columns] *= diagonal[rowStart + i/rows];
        }
    }
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            sum[index] += matrix[rowStart + i/rows][columnStart + j%columns];
        }
    }
}

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

int main(){
    getMatrix();

    pthread_t threads[nThreads];
    int *diagonal;
    diagonal = malloc(size * sizeof(int));

    for(int i = 0; i < size ; i++) {
        diagonal[i] = matrix[i][i];
    }
    time_t before, after;

    before = time(NULL);
    for(int i = 0; i < nThreads; i++) {
        pthread_args matrixAndIndex;
        matrixAndIndex.index = i;
        matrixAndIndex.diagonal = diagonal;
        pthread_create(threads[i], NULL, partialSum, (void *) &matrixAndIndex);
    }

    for(int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("The sum of the final Matrix is \n", sum[0] + sum[1] + sum[2] + sum[3]);
    after = time(NULL);

    printf("The time was %d seconds", after - before);

    free(matrix);
    free(diagonal);

    return 0;
}
 