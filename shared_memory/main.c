#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

struct SharedData {
    int number_of_threads;
    int matrix_size;
    bool terminate;
    double precision;
    double **current_matrix;
    double **next_matrix;
    double *delta;
    pthread_barrier_t barrier;
};

struct ThreadData {
    int thread_id;
};

struct ThreadContext {
    struct ThreadData thread_data;
    struct SharedData *shared_data;
};

void freeMatrixRowsMemory(double **matrix, int size);

void freeMatrixMemory(double **matrix, int size);

void
copyMatrix(double **original_matrix, double **destination_matrix, int size);

void smoothMatrix(double **matrix, double **smoothed_matrix, int size,
                  int number_of_threads, double precision);

double **mallocMatrix(int size);

void mallocMatrixRows(double **matrix, int size);

void setMatrixElementsTo(double **matrix, int size, double value);

double absoluteValue(double value);

double getAverageOfAdjacentValues(double **current_matrix, int row, int column);

bool isEdgeCoordinate(int row, int column, int size);

void *thread_function(void *arg);

void smoothElement(struct ThreadContext *thread_context, int row, int column);

struct SharedData
getSharedData(int size, int number_of_threads, double precision);

bool
shouldTerminate(double *delta_array, int number_of_threads, double precision);

void setLeftAndUpperTo(double **matrix, int size, double value);

int runSmoother(int size, int number_of_threads, double precision);

void freeMemoryOfSharedData(struct SharedData *sharedData);

int testScalabilityOfThreads();

double getEfficiency(double time_for_one_thread, double average_time,
                     int number_of_threads);

double getAverageTime(const double *times, int number_of_times);

void testScalabilityOfSize(int number_of_threads);

int main() {
    int best_number_of_threads;

    best_number_of_threads = testScalabilityOfThreads();
    printf("Best Thread Size is: %i\n", best_number_of_threads);
    testScalabilityOfSize(best_number_of_threads);
    return 0;
}

int testScalabilityOfThreads() {
    int size = 500;
    int runs = 1;
    int max_threads = 10;
    double precision = 0.01;
    double times[runs], average_time, time_of_one_thread;
    double shortest_average = 100000; // Arbitrary high number
    int fastest_number_of_threads = 1;

    struct timeval start, end;


    // Code to measure the time for
    printf("Test for Scaling of Threads\nThreads\tSize\tTime\t\tEff\n");

    for (int threads = 1; threads < max_threads; ++threads) {
        for (int run = 0; run < runs; ++run) {
            gettimeofday(&start, NULL);
            runSmoother(size, threads, precision);
            gettimeofday(&end, NULL);
            times[run] = (end.tv_sec - start.tv_sec) +
                         (end.tv_usec - start.tv_usec) / 1e6;
        }
        average_time = getAverageTime(times, runs);
        if (threads == 1) { time_of_one_thread = average_time; }
        if (average_time < shortest_average) {
            fastest_number_of_threads = threads;
            shortest_average = average_time;
        }
        printf("%i,\t%i,\t%f\t%f\t\n", threads, size, average_time,
               getEfficiency(time_of_one_thread, average_time, threads));
    }

    return fastest_number_of_threads;
}

void testScalabilityOfSize(int number_of_threads) {
    int multiplier = 50;
    int size;
    int runs = 1;
    double times[runs], average_time;
    double precision = 0.01;

    struct timeval start, end;
    printf("Test for Scaling of Size\nThreads\tSize\tTime\n");
    for (int multiple = 0; multiple < multiplier; ++multiple) {
        for (int run = 0; run < runs; ++run) {
            size = 100 * multiple;
            gettimeofday(&start, NULL);
            runSmoother(size, number_of_threads, precision);
            gettimeofday(&end, NULL);
            times[run] = (end.tv_sec - start.tv_sec) +
                         (end.tv_usec - start.tv_usec) / 1e6;
        }
        average_time = getAverageTime(times, runs);
        printf("%i,\t%i,\t%f\t\n", number_of_threads, size, average_time);
    }
}

double getEfficiency(double time_for_one_thread, double average_time,
                     int number_of_threads) {
    return ((time_for_one_thread) / average_time) / (double) number_of_threads;
}

double getAverageTime(const double *times, int number_of_times) {
    double total_time = 0;
    for (int index = 0; index < number_of_times; ++index) {
        total_time += times[index];
    }
    return (total_time) / ((double) number_of_times);
}

struct SharedData
getSharedData(int size, int number_of_threads, double precision) {
    struct SharedData shared_data;
    shared_data.matrix_size = size;
    shared_data.number_of_threads = number_of_threads;
    shared_data.precision = precision;
    shared_data.terminate = false;
    shared_data.current_matrix = mallocMatrix(size);
    shared_data.next_matrix = mallocMatrix(size);
    shared_data.delta = (double *) malloc(number_of_threads * sizeof(double));


    pthread_barrier_init(&shared_data.barrier, NULL, number_of_threads);
    setMatrixElementsTo(shared_data.current_matrix, shared_data.matrix_size, 0);
    setMatrixElementsTo(shared_data.next_matrix, shared_data.matrix_size, 0);

    return shared_data;
}

int runSmoother(int size, int number_of_threads, double precision) {
    double **matrix = mallocMatrix(size);
    double **smoothed_matrix = mallocMatrix(size);


    setMatrixElementsTo(matrix, size, 0);
    setMatrixElementsTo(smoothed_matrix, size, 0);
    setLeftAndUpperTo(matrix, size, 1);

    smoothMatrix(matrix, smoothed_matrix, size, number_of_threads, precision);

    freeMatrixMemory(matrix, size);
    freeMatrixMemory(smoothed_matrix, size);
    return 0;
}

void smoothMatrix(double **matrix, double **smoothed_matrix, int size,
                  int number_of_threads, double precision) {
    pthread_t threads[number_of_threads];
    struct ThreadContext thread_contexts[number_of_threads];
    struct SharedData shared_data = getSharedData(size, number_of_threads,
                                                  precision);

    copyMatrix(matrix, shared_data.current_matrix, size);

    for (int i = 0; i < number_of_threads; ++i) {
        thread_contexts[i].thread_data.thread_id = i;
        thread_contexts[i].shared_data = &shared_data;
        pthread_create(&threads[i], NULL, thread_function,
                       (void *) &thread_contexts[i]);
    }

    for (int i = 0; i < number_of_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&shared_data.barrier);
    copyMatrix(shared_data.current_matrix, smoothed_matrix,
               shared_data.matrix_size);
    freeMemoryOfSharedData(&shared_data);
}

void setLeftAndUpperTo(double **matrix, int size, double value) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            if (row == 0 || column == 0) {
                matrix[row][column] = value;
            }
        }
    }
}

void freeMemoryOfSharedData(struct SharedData *sharedData) {
    freeMatrixMemory(sharedData->current_matrix, sharedData->matrix_size);
    freeMatrixMemory(sharedData->next_matrix, sharedData->matrix_size);
    free(sharedData->delta);
}

void *thread_function(void *arg) {
    struct ThreadContext *thread_context = (struct ThreadContext *) arg;
    int matrix_size = thread_context->shared_data->matrix_size;
    int number_of_elements = matrix_size * matrix_size;
    int thread_id = thread_context->thread_data.thread_id;
    int number_of_threads = thread_context->shared_data->number_of_threads;
    int row, column, flattened_index;

    while (!thread_context->shared_data->terminate) {
        for (int multiple = 0; multiple * number_of_threads + thread_id <
                               number_of_elements; ++multiple) {
            flattened_index = multiple * number_of_threads + thread_id;
            row = flattened_index / matrix_size;
            column = flattened_index % matrix_size;
            smoothElement(thread_context, row, column);
        }

        pthread_barrier_wait(&thread_context->shared_data->barrier);

        if (thread_id == 0) {
            thread_context->shared_data->terminate = shouldTerminate(
                    thread_context->shared_data->delta,
                    number_of_threads, thread_context->shared_data->precision);

            for (int index = 0; index < number_of_threads; ++index) {
                thread_context->shared_data->delta[index] = 0;
            }

            double **temp_pointer = thread_context->shared_data->current_matrix;
            thread_context->shared_data->current_matrix =
                    thread_context->shared_data->next_matrix;
            thread_context->shared_data->next_matrix = temp_pointer;

        }
        pthread_barrier_wait(&thread_context->shared_data->barrier);
    }

    return NULL;
}

bool
shouldTerminate(double *delta_array, int number_of_threads, double precision) {
    for (int index = 0; index < number_of_threads; ++index) {
        if (precision < delta_array[index]) {
            return false;
        }
    }
    return true;
}

void smoothElement(struct ThreadContext *thread_context, int row, int column) {

    double average_adjacent_values, delta;
    double **current_matrix, **next_matrix;
    current_matrix = thread_context->shared_data->current_matrix;
    next_matrix = thread_context->shared_data->next_matrix;

    if (isEdgeCoordinate(row, column,
                         thread_context->shared_data->matrix_size)) {
        next_matrix[row][column] = current_matrix[row][column];
        delta = 0;
    } else {
        average_adjacent_values = getAverageOfAdjacentValues(current_matrix,
                                                             row, column);
        next_matrix[row][column] = average_adjacent_values;
        delta = absoluteValue(
                current_matrix[row][column] - average_adjacent_values);
    }
    if (thread_context->shared_data->delta[thread_context->thread_data.thread_id] <
        delta) {
        thread_context->shared_data->delta[thread_context->thread_data.thread_id] = delta;
    }
}

void
copyMatrix(double **original_matrix, double **destination_matrix, int size) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            destination_matrix[row][column] = original_matrix[row][column];
        }
    }
}

void setMatrixElementsTo(double **matrix, int size, double value) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            matrix[row][column] = value;
        }
    }
}


void freeMatrixMemory(double **matrix, int size) {
    setMatrixElementsTo(matrix, size, 0);
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

void mallocMatrixRows(double **matrix, int size) {
    for (int row = 0; row < size; row++) {
        matrix[row] = (double *) malloc(size * sizeof(double));
        if (matrix[row] == NULL) {
            fprintf(stderr, "Memory allocation failed inner array\n");
            exit(2);
        }
    }
}

double
getAverageOfAdjacentValues(double **current_matrix, int row, int column) {
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
