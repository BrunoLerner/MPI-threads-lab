#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>
#include <sys/time.h>

#define size 16000
#define nThreads 4

struct thread_args {
    int index;
    int ** matrix;
    int * diagonal;
};

int sum[4], **matrix;

void * partialSum (void *matrixAndIndex) {
    struct thread_args * args = (struct thread_args *) matrixAndIndex;
    int index = args->index;
    int *diagonal = args->diagonal;
    int **matrix = args->matrix;
    int columns = size/2, rows = size/2, rowStart = 0, columnStart = 0;

    if (index == 1) columnStart = size/2;   
    else if (index == 2) rowStart = size/2;
    else if (index == 3) {
        rowStart = size/2;
        columnStart = size/2;
    }

    for (int i = rowStart; i < rowStart + rows; i++) {
        for (int j = columnStart; j < columnStart + columns; j++) {
            matrix[i][j] *= diagonal[i];
            sum[index] += matrix[i][j];
        }
    }
    printf("Thread %d diz: Aqui deu %d.\n", index, sum[index]);
}

int ** getMatrix() {
    // Alocando espaço pra matriz
    int **matrix = malloc(size * sizeof(int *));
    
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

int main(){
    int **matrix = getMatrix();

    pthread_t threads[nThreads];
    int *diagonal = malloc(size * sizeof(int));

    for(int i = 0; i < size ; i++) {
        diagonal[i] = matrix[i][i];
    }
    struct timeval startTime, endTime;

    gettimeofday(&startTime, NULL);

    struct thread_args * matrixAndIndex;
    for(int i = 0; i < nThreads; i++) {
        matrixAndIndex = malloc(sizeof(struct thread_args));
        matrixAndIndex->index = i;
        matrixAndIndex->matrix = matrix;
        matrixAndIndex->diagonal = diagonal;
        pthread_create(&threads[i], NULL, partialSum, (void *) matrixAndIndex);
    }

    for(int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&endTime, NULL);
    double time = (endTime.tv_sec*1000000 + endTime.tv_usec) - (startTime.tv_sec*1000000 +  startTime.tv_usec);
    
    printf("O resultado da soma total é %d \n O tempo foi %lf \n", sum[0] + sum[1] + sum[2] + sum[3], time/1000000.0);

    free(matrix);
    free(diagonal);

    return 0;
}
 
