#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>
#include <sys/time.h>


#define size 16000
#define nThreads 2

struct thread_args {
    int index;
    int **matrix;
};

int sum[2];

void * partialSum (void *matrixAndIndex) {
    struct thread_args * args = (struct thread_args *) matrixAndIndex;
    int index = args->index;
    int **matrix= args->matrix;
    int columns = size/2, rows = size, start = 0;

    if(index == 1) start = size/2;

    for(int i = 0; i < rows; i++) {
        for(int j = start; j < start + columns; j++) {
            matrix[i][j] *= matrix[i][i];
            sum[index] += matrix[i][j];
        }
    }
    printf("Thread %d diz: Aqui deu %d.\n", index, sum[index]);
}

int** getMatrix() {
    // Alocando espaço pra matriz
    int **matrix = malloc(size * sizeof(int *));
    
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

    return matrix;
}

int main(){
    int **matrix = getMatrix();
    
    struct timeval startTime, endTime;
    pthread_t threads[nThreads];

    gettimeofday(&startTime, NULL);
    struct thread_args* matrixAndIndex;
    for(int i = 0; i < nThreads; i++) {
        matrixAndIndex = malloc(sizeof(struct thread_args));
        matrixAndIndex->index = i;
        matrixAndIndex->matrix = matrix;
        pthread_create(&threads[i], NULL, partialSum, (void *) matrixAndIndex);
    }
    
    for(int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    double time = (endTime.tv_sec*1000000 + endTime.tv_usec) - (startTime.tv_sec*1000000 +  startTime.tv_usec);

    printf("O resultado da soma total é %d \n O tempo foi %lf\n", sum[0] + sum[1], time/1000000.0);
    free(matrix);

    return 0;
} 
