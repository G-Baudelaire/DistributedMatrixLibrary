#include <stdio.h>
#include <pthread.h>
#include <malloc.h>


#define NUM_THREADS 3


/// Free each row (array) of memory in the matrix.
/// \param matrix
/// \param rows
void freeMatrixRowsMemory(double **matrix, int size);

/// Free the memory of the matrix when it is not in use anymore.
/// \param matrix
/// \param rows
void freeMatrixMemory(double **matrix, int size);

/// Make every element in the matrix zero.
/// \param matrix
/// \param size
void makeMatrixElementsZero(double **matrix, int size);

void *thread_function(void *arg);

pthread_barrier_t barrier;

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize the barrier with the number of threads
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);

    return 0;
}

void makeMatrixElementsZero(double **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            matrix[row][column] = 0;
        }
    }
}

void freeMatrixMemory(double **matrix, int size) {
    makeMatrixElementsZero(matrix, size);
    freeMatrixRowsMemory(matrix, size);
    free(matrix);
}

void freeMatrixRowsMemory(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
}

void *thread_function(void *arg) {
    int thread_id = *(int *) arg;

    printf("Thread %d is doing some work...\n", thread_id);

    // Wait at the barrier
    pthread_barrier_wait(&barrier);

    printf("Thread %d continues its work after the barrier.\n", thread_id);

    return NULL;
}




//int main() {
//    int rows = 5;
//    int columns = 5;
//    double **matrix = allocateMatrixMemory(rows, columns);
//    makeMatrixElementsZero(matrix, rows, columns);
//}

//struct number {
//    int uniqueNumber;
//};

//void *showUniqueNumber(void *numberStruct);

//int main()
//{
//    pthread_t threadID[NUM_THREADS];
//    struct number numbers[NUM_THREADS];
//
//    for(int i = 0; i < NUM_THREADS; i++)
//    {
//        numbers[i].uniqueNumber = i;
//        pthread_create(&threadID[i], NULL, showUniqueNumber, numbers + i);
//    }
//
//    for(int i=0; i < NUM_THREADS; i++)
//    {
//        pthread_join(threadID[i], NULL);
//    }
//
//    return 0;
//}

//void *showUniqueNumber(void *numberStruct) {
//    struct number *threadPtrN = numberStruct;
//
//    printf("%i\n", threadPtrN->uniqueNumber);
//
//    return (void *) 0;
//}
