#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#define size 16000
#define nThreads 4

typedef struct threads_param {
    int index;
    int ** matrix;
} param;

int *partialSum (void *matrixAndIndex) {
    param *args = (param *) matrixAndIndex;
    int index = args.index;
    int **matrix = args.matrix;
    int start = 0;
    
    
    // return sum;
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

    

    pthread_t threads[nThreads];

    for(int i = 0; i < nThreads; i++) {
        param matrixAndIndex;

        matrixAndIndex.matrix = &matrix;
        matrixAndIndex.index = i;

        pthread_create(threads[i], NULL, partialSum, &matrixAndIndex);
    }

    for(int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("After Thread\n");


    return 0;
}
 
