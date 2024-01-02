//
// Created by Germain Jones on 28/12/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

// Error Codes
enum ErrorCodes {
    ROW_MALLOC_FAILURE = 1,
    COLUMN_MALLOC_FAILURE = 2,
    NOT_ENOUGH_PROCESSES = 3,
};

void printMatrix(double **current_matrix, int size);

double absoluteValue(double value);

void setValuesToZero(double **matrix, int size);

double **mallocMatrix(int size);

void freeMatrixMemory(double **current_matrix, int rows);

bool isEdgeCoordinate(int row, int column, int size);

void copyMatrix(double **original, double **destination, int size);

double getAverageOfAdjacentValues(double **current_matrix, int row, int column);

void sequentialSolver(double **matrix, double **output_matrix, int size,
                      double precision, bool print_step);

void setLeftAndUpperTo(double **matrix, int size, double value);

double **mallocMatrixRows(int size);

void mallocMatrixColumns(double **matrix, int size);

void rank_0_logic();

void compute_rows_to_smooth(int matrix_size, int *output_array,
                            int num_workers);

void compute_partition_start(int *output_array, int *rows_to_smooth,
                             int num_workers);

void compute_partition_end(int *output_array, int *rows_to_smooth,
                           int num_workers);

void compute_flattened_start(int *output_array, int *partition_start,
                             int matrix_size, int num_workers);

void compute_flattened_end(int *output_array, int *partition_end,
                           int matrix_size, int num_workers);

void compute_counts(int *output_array, int *flattened_start, int *flattened_end,
                    int num_workers);

void flatten_matrix(double **input_matrix, double *output_flattend_array,
                    const int matrix_size);

// int main(int argc, char *argv[]) {
//     // Check if the correct number of command-line arguments is provided
//     if (argc != 3) {
//         fprintf(stderr, "Usage: %s -n <number>\n", argv[0]);
//         return 1; // Return an error code
//     }
//
//     // Check if the first argument is the flag "-n"
//     if (strcmp(argv[1], "-n") != 0) {
//         fprintf(stderr, "Usage: %s -n <number>\n", argv[0]);
//         return 1; // Return an error code
//     }
//
//     // Convert the second argument (number) to an integer
//     int number = atoi(argv[2]);
//
//     // Use the integer in your program
//     printf("The provided number is: %d\n", number);
//
//     return 0; // Return success

// }
void printArrayInt(int *array, int array_len);

void printArrayDouble(double *array, double array_len);

void setup(int num_worker_processes, int matrix_size);

void setup(int num_worker_processes, int matrix_size) {
    int step;
    const int rows_per_thread = matrix_size / num_worker_processes;
    const int remainder = matrix_size % num_worker_processes;
    int starting_row[num_worker_processes];
    int ending_row[num_worker_processes];

    starting_row[0] = 0;
    for (int i = 1; i < num_worker_processes; ++i) {
        if (i - 1 < remainder)
            starting_row[i] = starting_row[i - 1] + rows_per_thread + 1;
        else {
            starting_row[i] = starting_row[i - 1] + rows_per_thread;
        }
    }

    printArrayInt(starting_row, num_worker_processes);
}

void printArrayInt(int *array, int array_len) {
    printf("[");

    for (int i = 0; i < array_len; ++i) {
        printf("%i, ", array[i]);
    }

    printf("]\n");
}

void printArrayDouble(double *array, double array_len) {
    printf("[");

    for (int i = 0; i < array_len; ++i) {
        printf("%f, ", array[i]);
    }

    printf("]\n");
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    const int SIZE = 3;
    const double PRECISION = 2;
    int num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    int num_workers = num_processes - 1;

    int task_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

    int rows_to_smooth[num_workers];
    int partition_start[num_workers], partition_end[num_workers];
    int flattened_start[num_workers], flattened_end[num_workers];
    int counts[num_processes];

    compute_rows_to_smooth(SIZE, rows_to_smooth, num_workers);
    compute_partition_start(partition_start, rows_to_smooth, num_workers);
    compute_partition_end(partition_end, rows_to_smooth, num_workers);
    compute_flattened_start(flattened_start, partition_start, SIZE,
                            num_workers);
    compute_flattened_end(flattened_end, partition_end, SIZE, num_workers);
    compute_counts(counts, flattened_start, flattened_end, num_workers);

    int displacement[num_processes];
    displacement[0] = 0;
    for (int i = 1; i < num_processes; ++i) {
        displacement[i] = flattened_start[i - 1];
    }
    double *send_buffer;
    double *recv_buffer;

    if (task_id == 0) {
        send_buffer = malloc(SIZE * SIZE * sizeof(double));
        for (int i = 0; i < SIZE * SIZE; ++i) {
            send_buffer[i] = i;
        }
    } else {
        recv_buffer = malloc(counts[task_id] * sizeof(double));
    }

    MPI_Scatterv(send_buffer, counts, displacement, MPI_DOUBLE,
                 recv_buffer, counts[task_id], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (task_id == 1) {
        // printf("Process 1 has array\n");
        // printArrayDouble(recv_buffer, counts[task_id]);
    }

    bool terminate = false;
    double delta;
    while (task_id == 0 && !terminate) {
    }

    while (task_id != 0 && !terminate) {

    }
    // Teardown
    if (task_id == 0) {
        free(send_buffer);
    } else {
        free(recv_buffer);
    }

    MPI_Finalize();
    return 0;
}

void flatten_matrix(double **input_matrix, double *output_flattend_array,
                    const int matrix_size) {
    int flattened_index;
    for (int row = 0; row < matrix_size; ++row) {
        for (int column = 0; column < matrix_size; ++column) {
            flattened_index = row * matrix_size + column;
            output_flattend_array[flattened_index] = input_matrix[row][column];
        }
    }
}

// int main(int argc, char* argv[]) {
//     MPI_Init(&argc, &argv);
//
//
//     int world_rank, world_size;
//
//     MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//     int send_data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//     int recv_data[4];
//
//     int sendcounts[4] = {2, 3, 3, 1};
//     int displs[4] = {0, 2, 2, 9};
//
//     MPI_Scatterv(send_data, sendcounts, displs, MPI_INT, recv_data, 4, MPI_INT, 0, MPI_COMM_WORLD);
//
//     for (int i = 0; i < 4; i++) {
//         printf("Process %d\tPosition:%i\tData %d\n", world_rank, i,recv_data[i]);
//     }
//
//     MPI_Finalize();
//     return 0;
// }

void compute_rows_to_smooth(int matrix_size, int *output_array,
                            int num_workers) {
    const int rows_per_thread = matrix_size / num_workers;
    const int remainder = matrix_size % num_workers;

    for (int i = 0; i < num_workers; ++i) {
        if (i < remainder) {
            output_array[i] = rows_per_thread + 1;
        } else {
            output_array[i] = rows_per_thread;
        }
    }
}

void compute_partition_start(int *output_array, int *rows_to_smooth,
                             int num_workers) {
    if (num_workers < 3) {
        exit(NOT_ENOUGH_PROCESSES);
    }

    output_array[0] = 0;
    for (int i = 1; i < num_workers; ++i) {
        output_array[i] = output_array[i - 1] + rows_to_smooth[i - 1];
    }

    for (int i = 1; i < num_workers; ++i) {
        output_array[i] -= 1;
    }
}

void compute_partition_end(int *output_array, int *rows_to_smooth,
                           int num_workers) {
    if (num_workers < 3) {
        exit(NOT_ENOUGH_PROCESSES);
    }

    output_array[0] = rows_to_smooth[0];
    for (int i = 1; i < num_workers; ++i) {
        output_array[i] = output_array[i - 1] + rows_to_smooth[i];
    }

    // Last row does have a row after it to check.
    output_array[num_workers - 1] -= 1;
}

void compute_flattened_start(int *output_array, int *partition_start,
                             int matrix_size, int num_workers) {
    for (int i = 0; i < num_workers; ++i) {
        output_array[i] = partition_start[i] * matrix_size;
    }
}

void compute_flattened_end(int *output_array, int *partition_end,
                           int matrix_size, int num_workers) {
    for (int i = 0; i < num_workers; ++i) {
        output_array[i] = matrix_size * partition_end[i] + matrix_size - 1;
    }
}

void compute_counts(int *output_array, int *flattened_start, int *flattened_end,
                    int num_workers) {
    // counts[0] = 0 because the manager does not need data.
    output_array[0] = 0;
    for (int i = 0; i < num_workers; ++i) {
        output_array[i + 1] = flattened_end[i] - flattened_start[i] + 1;
    }
}

void rank_0_logic() {
    double **matrix, **output_matrix;
    int size = 4;
    double precision = 0.1;

    matrix = mallocMatrix(size);
    output_matrix = mallocMatrix(size);
    setLeftAndUpperTo(matrix, size, 1);
    sequentialSolver(matrix, output_matrix, size, precision, false);
    freeMatrixMemory(matrix, size);
    freeMatrixMemory(output_matrix, size);
}

void setValuesToZero(double **matrix, int size) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            matrix[row][column] = 0;
        }
    }
}

double **mallocMatrix(int size) {
    double **matrix = mallocMatrixRows(size);
    mallocMatrixColumns(matrix, size);
    setValuesToZero(matrix, size);
    return matrix;
}

double **mallocMatrixRows(int size) {
    // ReSharper disable once CppDFAMemoryLeak
    double **matrix = malloc(size * sizeof(double *));

    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for rows\n");
        exit(ROW_MALLOC_FAILURE);
    }

    return matrix;
}

void mallocMatrixColumns(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = (double *) malloc(size * sizeof(double));

        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns\n");
            exit(2);
        }
    }
}

void freeMatrixMemory(double **current_matrix, int rows) {
    for (int i = 0; i < rows; i++) { free(current_matrix[i]); }
    free(current_matrix);
}

void sequentialSolver(double **matrix,
                      double **output_matrix,
                      int size,
                      double precision,
                      bool print_step) {
    bool terminate = false;
    double sumOfAdjacentValues, newValue, delta;
    double **nextMatrix = mallocMatrix(size);

    copyMatrix(matrix, output_matrix, size);

    while (!terminate) {
        terminate = true;

        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                if (!isEdgeCoordinate(row, column, size)) {
                    sumOfAdjacentValues = getAverageOfAdjacentValues(
                        output_matrix, row, column);
                    newValue = sumOfAdjacentValues / 4.0;
                    nextMatrix[row][column] = newValue;
                    delta = absoluteValue(
                        output_matrix[row][column] - newValue);
                    if (delta > precision) {
                        terminate = false;
                    }
                }
            }
        }
        if (print_step) { printMatrix(nextMatrix, size); }
        copyMatrix(nextMatrix, output_matrix, size);
    }

    freeMatrixMemory(nextMatrix, size);
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

void copyMatrix(double **original, double **destination, int size) {
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            destination[row][column] = original[row][column];
        }
    }
}

double getAverageOfAdjacentValues(double **current_matrix, int row,
                                  int column) {
    return current_matrix[row][column + 1] + current_matrix[row + 1][column] +
           current_matrix[row][column - 1] + current_matrix[row - 1][column];
}

double absoluteValue(double value) {
    if (value < 0) { return -value; }
    return value;
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
    };
}

void printMatrix(double **current_matrix, int size) {
    int row, columns;

    for (row = 0; row < size; row++) {
        for (columns = 0; columns < size; columns++) {
            printf("%f     ", current_matrix[row][columns]);
        }
        printf("\n");
    }
    printf("\n");
}
