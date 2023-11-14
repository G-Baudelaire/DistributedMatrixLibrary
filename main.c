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

void smoothMatrix(double **matrix, double **smoothed_matrix, int size, int number_of_threads, double precision);

double **mallocMatrix(int size);

void **mallocMatrixRows(double **matrix, int size);

void printMatrix(double **matrix, int size);

void setMatrixElementsToZero(double **matrix, int size);

void setMatrixEdgeElementsTo(double **matrix, int size, double edge_value);

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
double absoluteValue(double value);

double getAverageOfAdjacentValues(double **current_matrix, int row, int column);

bool isEdgeCoordinate(int row, int column, int size);

struct SharedData {
    int number_of_threads;
    int matrix_size;
    pthread_barrier_t barrier;
    double **current_matrix;
    double **next_matrix;
    pthread_mutex_t delta_mutex;
    double delta;
    bool terminate;
    double precision;
};

struct ThreadData {
    int thread_id;
};

struct ThreadContext {
    struct ThreadData thread_data;
    struct SharedData *shared_data;
};

void *thread_function(void *thread_data);


void smoothElement(struct ThreadContext *thread_context, int row, int column);

struct SharedData getSharedData(int size, int number_of_threads, double precision);

void updateDelta(struct ThreadContext *thread_context, double delta);


int main() {
    int size = 12;
    int number_of_threads = 5;

    double **matrix = mallocMatrix(size);
    double **smoothed_matrix = mallocMatrix(size);

    // Set initial matrices values
    setMatrixElementsToZero(matrix, size);
    setMatrixElementsToZero(smoothed_matrix, size);
    setMatrixEdgeElementsTo(matrix, size, 1);

    printf("Original Matrix:\n");
    printMatrix(matrix, size);

    smoothMatrix(matrix, smoothed_matrix, size, number_of_threads, 0.01);

    printf("Smoothed Matrix:\n");
    printMatrix(smoothed_matrix, size);

    // Free memory of matrices
    freeMatrixMemory(matrix, size);
    freeMatrixMemory(smoothed_matrix, size);
    return 0;
}

struct SharedData getSharedData(int size, int number_of_threads, double precision) {
    struct SharedData shared_data;
    shared_data.matrix_size = size;
    shared_data.number_of_threads = number_of_threads;
    shared_data.precision = precision;
    shared_data.delta = 0;
    shared_data.delta_mutex = PTHREAD_MUTEX_INITIALIZER;
    shared_data.terminate = false;
    shared_data.current_matrix = mallocMatrix(size);
    shared_data.next_matrix = mallocMatrix(size);

    pthread_barrier_init(&shared_data.barrier, NULL, number_of_threads);
    setMatrixElementsToZero(shared_data.current_matrix, shared_data.matrix_size);
    setMatrixElementsToZero(shared_data.next_matrix, shared_data.matrix_size);

    return shared_data;
}

void freeMemoryOfSharedData(struct SharedData *sharedData);

void freeMemoryOfSharedData(struct SharedData *sharedData) {
    freeMatrixMemory(sharedData->current_matrix, sharedData->matrix_size);
    freeMatrixMemory(sharedData->next_matrix, sharedData->matrix_size);
}

void smoothMatrix(double **matrix, double **smoothed_matrix, int size, int number_of_threads, double precision) {
    pthread_t threads[number_of_threads];
    struct ThreadContext thread_contexts[number_of_threads];
    struct SharedData shared_data = getSharedData(size, number_of_threads, precision);

    copyMatrix(matrix, shared_data.current_matrix, size);

    // Create threads
    for (int i = 0; i < number_of_threads; ++i) {
        thread_contexts[i].thread_data.thread_id = i;
        thread_contexts[i].shared_data = &shared_data;
        pthread_create(&threads[i], NULL, thread_function, (void *) &thread_contexts[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&shared_data.barrier);
    copyMatrix(shared_data.current_matrix, smoothed_matrix, shared_data.matrix_size);
    freeMemoryOfSharedData(&shared_data);
}

void *thread_function(void *arg) {
    struct ThreadContext *thread_context = (struct ThreadContext *) arg;
    int matrix_size = thread_context->shared_data->matrix_size;
    int number_of_elements = matrix_size * matrix_size;
    int thread_id = thread_context->thread_data.thread_id;
    int number_of_threads = thread_context->shared_data->number_of_threads;
    int row, column, flattened_index;

    while (!thread_context->shared_data->terminate) {
        for (int multiple = 0; multiple * number_of_threads + thread_id < number_of_elements; ++multiple) {
            flattened_index = multiple * number_of_threads + thread_id;
            row = flattened_index / matrix_size;
            column = flattened_index % matrix_size;
            smoothElement(thread_context, row, column);
        }

        pthread_barrier_wait(&thread_context->shared_data->barrier);


        if (thread_id == 0) {
            if (thread_context->shared_data->delta < thread_context->shared_data->precision) {
                thread_context->shared_data->terminate = true;
            }
            thread_context->shared_data->delta = 0;
            copyMatrix(thread_context->shared_data->next_matrix,
                       thread_context->shared_data->current_matrix,
                       matrix_size);
//            printMatrix(thread_context->shared_data->current_matrix, thread_context->shared_data->matrix_size);
        }
        pthread_barrier_wait(&thread_context->shared_data->barrier);
    }

    return NULL;
}

void smoothElement(struct ThreadContext *thread_context, int row, int column) {
    double average_adjacent_values, delta;
    double **current_matrix, **next_matrix;
    current_matrix = thread_context->shared_data->current_matrix;
    next_matrix = thread_context->shared_data->next_matrix;

    if (isEdgeCoordinate(row, column, thread_context->shared_data->matrix_size)) {
        next_matrix[row][column] = current_matrix[row][column];
        delta = 0;
    } else {
        average_adjacent_values = getAverageOfAdjacentValues(current_matrix, row, column);
        next_matrix[row][column] = average_adjacent_values;
        delta = absoluteValue(current_matrix[row][column] - average_adjacent_values);
    }

    updateDelta(thread_context, delta);
}

void updateDelta(struct ThreadContext *thread_context, double delta) {
    // Lock the mutex before accessing the critical section
    pthread_mutex_lock(&thread_context->shared_data->delta_mutex);

    if (thread_context->shared_data->delta < delta) {
        thread_context->shared_data->delta = delta;
    }

    // Unlock the mutex after finishing the critical section
    pthread_mutex_unlock(&thread_context->shared_data->delta_mutex);
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

void setMatrixEdgeElementsTo(double **matrix, int size, double edge_value) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            if (isEdgeCoordinate(row, column, size)) {
                matrix[row][column] = edge_value;
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

double absoluteValue(double value) {
    if (value < 0) {
        return -value;
    } else {
        return value;
    }
}

