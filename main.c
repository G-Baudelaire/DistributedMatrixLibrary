#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdbool.h>


/// Free each row (array) of memory in the current_matrix.
/// \param matrix
/// \param rows
void freeMatrixRowsMemory(double **matrix, int size);

/// Free the memory of the current_matrix when it is not in use anymore.
/// \param matrix
/// \param rows
void freeMatrixMemory(double **matrix, int size);

/// Make every element in the current_matrix zero.
/// \param matrix
/// \param size
void makeMatrixElementsZero(double **matrix, int size);

void copyMatrix(double **original_matrix, double **destination_matrix, int size);

void smoothMatrix(double **matrix, double **smoothed_matrix, int size, int number_of_threads);

double **mallocMatrix(int size);

void **mallocMatrixRows(double **matrix, int size);

void printMatrix(double **matrix, int size);

void setMatrixElementsToZero(double **matrix, int size);

void setMatrixEdgeElementsToOne(double **matrix, int size);

double getAverageOfAdjacentValues(double **current_matrix, int row, int column);

bool isEdgeCoordinate(int row, int column, int size);

struct SharedThreadData {
    int number_of_threads, matrix_size;
    pthread_barrier_t barrier;
    double **current_matrix, **next_matrix;
};

struct UniqueThreadData {
    int thread_id;
    struct SharedThreadData shared_thread_data;
};

void *thread_function(struct UniqueThreadData thread_data);


void smoothElement(struct SharedThreadData shared_thread_data, int row, int column);

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

//void* myThreadFunction(void* arg) {
//    // Lock the mutex before accessing the critical section
//    printf("Thread %ld is in the starting section\n", (long)arg);
//
//    pthread_mutex_lock(&myMutex);
//    printf("Thread %ld is entering critical section\n", (long)arg);
//
//    // Critical section (protected by the mutex)
//    printf("Thread %ld is in the critical section\n", (long)arg);
//
//    printf("Thread %ld is exiting critical section\n", (long)arg);
//
//    // Unlock the mutex after finishing the critical section
//    pthread_mutex_unlock(&myMutex);
//
//    printf("Thread %ld is in the ending section\n", (long)arg);
//
//    // Other thread operations...
//
//    return NULL;
//}
//
//int main() {
//    const int num_threads = 5;
//    pthread_t threads[num_threads];
//
//    // Create and run threads
//    for (long i = 0; i < num_threads; ++i) {
//        pthread_create(&threads[i], NULL, myThreadFunction, (void*)i);
//    }
//
//    // Wait for all threads to finish
//    for (int i = 0; i < num_threads; ++i) {
//        pthread_join(threads[i], NULL);
//    }
//
//    // Destroy the mutex
//    pthread_mutex_destroy(&myMutex);
//
//    return 0;
//}

int main() {
    int size = 9;
    int number_of_threads = 5;

    double **matrix = mallocMatrix(size);
    double **smoothed_matrix = mallocMatrix(size);

    // Set initial matrices values
    setMatrixElementsToZero(matrix, size);
    setMatrixEdgeElementsToOne(matrix, size);
    setMatrixElementsToZero(smoothed_matrix, size);

    printMatrix(matrix, size);
    smoothMatrix(matrix, smoothed_matrix, size, number_of_threads);
    printMatrix(smoothed_matrix, size);

    // Free memory of matrices
    freeMatrixMemory(matrix, size);
    freeMatrixMemory(smoothed_matrix, size);
    return 0;
}

void smoothMatrix(double **matrix, double **smoothed_matrix, int size, int number_of_threads) {
    pthread_barrier_t barrier;
    pthread_t threads[number_of_threads];
    int thread_ids[number_of_threads];
    struct UniqueThreadData thread_data[number_of_threads];
    struct SharedThreadData shared_thread_data;
    double **current_matrix = mallocMatrix(size);
    double **nextMatrix = smoothed_matrix;
    double **deltaMatrix = mallocMatrix(size);
    copyMatrix(matrix, current_matrix, size);
    // Initialize the barrier with the number of threads
    pthread_barrier_init(&barrier, NULL, number_of_threads);

    // Fill shared thread data
    shared_thread_data.barrier = barrier;
    shared_thread_data.number_of_threads = number_of_threads;
    shared_thread_data.current_matrix = current_matrix;
    shared_thread_data.matrix_size = size;
    shared_thread_data.next_matrix = nextMatrix;

    // Create threads
    for (int i = 0; i < number_of_threads; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].shared_thread_data = shared_thread_data;
        pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_join(threads[i], NULL);
    }


    // Destroy the barrier
    pthread_barrier_destroy(&barrier);
}

void copyMatrix(double **original_matrix, double **destination_matrix, int size) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            destination_matrix[row][column] = original_matrix[row][column];
        }
    }
}

void setMatrixElementsToZero(double **matrix, int size) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            matrix[row][column] = 0;
        }
    }
}

void makeMatrixElementsZero(double **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            matrix[row][column] = 0;
        }
    }
}

void setMatrixEdgeElementsToOne(double **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            if (isEdgeCoordinate(row, column, size)) {
                matrix[row][column] = 1;
            } else {
                matrix[row][column] = 0;
            }
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

double **mallocMatrix(int size) {
    double **matrix = (double **) malloc(size * sizeof(double *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for outer array\n");
        exit(1);
    }

    // Allocate memory for each row
    mallocMatrixRows(matrix, size);
    return matrix;
}

void **mallocMatrixRows(double **matrix, int size) {
    for (int row = 0; row < size; row++) {
        matrix[row] = (double *) malloc(size * sizeof(double));
        if (matrix[row] == NULL) {
            fprintf(stderr, "Memory allocation failed inner array\n");
            exit(2);
        }
    }
}

void printMatrix(double **matrix, int size) {
    int row, column;

    for (row = 0; row < size; row++) {
        for (column = 0; column < size; column++) {
            printf("%f     ", matrix[row][column]);
        }
        printf("\n");
    }
    printf("\n");
}

void *thread_function(struct UniqueThreadData thread_data) {
    // TODO: Make this use multiples to avoid pointless checks
    int flattened_index;

    printf("Thread %d is doing some work...\n", thread_data.thread_id);

    for (int row = 0; row < thread_data.shared_thread_data.matrix_size; ++row) {
        for (int column = 0; column < thread_data.shared_thread_data.matrix_size; ++column) {
            flattened_index = row * thread_data.shared_thread_data.matrix_size + column;
            if (flattened_index % thread_data.shared_thread_data.number_of_threads == thread_data.thread_id) {
                smoothElement(thread_data.shared_thread_data, row, column);
            }
        }
    }

    // Wait at the barrier
    pthread_barrier_wait(&thread_data.shared_thread_data.barrier);

    printf("Thread %d continues its work after the barrier.\n", thread_data.thread_id);

    return NULL;
}

void smoothElement(struct SharedThreadData shared_thread_data, int row, int column) {
    double average_adjacent_values;

    if (isEdgeCoordinate(row, column, shared_thread_data.matrix_size)) {
        shared_thread_data.next_matrix[row][column] = shared_thread_data.current_matrix[row][column];
    } else {
        average_adjacent_values = getAverageOfAdjacentValues(shared_thread_data.current_matrix, row, column);
        shared_thread_data.next_matrix[row][column] = average_adjacent_values;
    }
}

double getAverageOfAdjacentValues(double **current_matrix, int row, int column) {
    return (current_matrix[row][column + 1] +
            current_matrix[row + 1][column] +
            current_matrix[row][column - 1] +
            current_matrix[row - 1][column]) / 4.0;
}

bool isEdgeCoordinate(int row, int column, int size) {
    if ((row == 0) || // First Row
        (row == size - 1) || // Last Row
        (column % size == 0) || // First Column
        (column % size == size - 1)) // Last Column
    {
        return true;
    } else {
        return false;
    }
}


//void *thread_function(struct UniqueThreadData thread_data) {
//    printf("Thread %d is doing some work...\n", thread_data.thread_id);
//    printf("%i \n", thread_data.barrier);
//    // Wait at the barrier
//    pthread_barrier_wait(&thread_data.barrier);
//
//    printf("Thread %d continues its work after the barrier.\n", thread_data.thread_id);
//
//    return NULL;
//}


//int main() {
//    pthread_t threads[NUM_THREADS];
//    int thread_ids[NUM_THREADS];
//
//    // Initialize the barrier with the number of threads
//    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
//
//    // Create threads
//    for (int i = 0; i < NUM_THREADS; ++i) {
//        thread_ids[i] = i;
//        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
//    }
//
//    // Wait for all threads to finish
//    for (int i = 0; i < NUM_THREADS; ++i) {
//        pthread_join(threads[i], NULL);
//    }
//
//    // Destroy the barrier
//    pthread_barrier_destroy(&barrier);
//
//    return 0;
//}
//int main() {
//    int rows = 5;
//    int columns = 5;
//    double **current_matrix = mallocMatrix(rows, columns);
//    makeMatrixElementsZero(current_matrix, rows, columns);
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
