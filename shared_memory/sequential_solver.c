////
//// Created by Germain Jones on 11/11/2023.
////
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//
//void printMatrix(double **current_matrix, int numberOfRows, int numberOfColumns);
//
//double absoluteValue(double value);
//
//double **mallocMatrix(int rows, int columns);
//
//void freeMatrixMemory(double **current_matrix, int rows);
//
//bool isEdgeCoordinate(int row, int column, int rows, int columns);
//
//void copyMatrix(double **original, double **destination, int rows, int columns);
//
//double getAverageOfAdjacentValues(double **current_matrix, int row, int column);
//
//double **sequentialSolver(double **current_matrix, int rows, int columns, double precision);
//
//
//double **mallocMatrix(int rows, int columns) {
//    double **current_matrix = (double **) malloc(rows * sizeof(double *));
//    double mat[][4] = {{1.0, 1.0, 1.0, 1.0},
//                       {1.0, 0.0, 0.0, 0.0},
//                       {1.0, 0.0, 0.0, 0.0},
//                       {1.0, 0.0, 0.0, 0.0},};
//    if (current_matrix == NULL) {
//        fprintf(stderr, "Memory allocation failed for rows\n");
//        exit(1);
//    }
//
//    // Allocate memory for each row
//    for (int i = 0; i < rows; i++) {
//        current_matrix[i] = (double *) malloc(columns * sizeof(double));
//        if (current_matrix[i] == NULL) {
//            fprintf(stderr, "Memory allocation failed for columns\n");
//            exit(2);
//        }
//    }
//
//    // Initialize with some value
//    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < columns; j++) {
//            if (i == 0 || j == 0){
//                current_matrix[i][j] = 1.0;
//            } else {
//                current_matrix[i][j] = 0.0;
//            }
//        }
//    }
//    return current_matrix;
//}
//
//void freeMatrixMemory(double **current_matrix, int rows) {
//    for (int i = 0; i < rows; i++) {
//        free(current_matrix[i]);
//    }
//    free(current_matrix);
//}
//
//double **sequentialSolver(double **current_matrix, int rows, int columns, double precision) {
//    bool terminate = false;
//    double sumOfAdjacentValues, newValue, delta;
//    double **current_matrix = mallocMatrix(rows, columns);
//    double **nextMatrix = mallocMatrix(rows, columns);
//
//    copyMatrix(current_matrix, current_matrix, rows, columns);
//
//    while (!terminate) {
//        terminate = true;
//
//        for (int row = 0; row < rows; ++row) {
//            for (int column = 0; column < columns; ++column) {
//                if (!isEdgeCoordinate(row, column, rows, columns)) {
//                    sumOfAdjacentValues = getAverageOfAdjacentValues(current_matrix, row, column);
//                    newValue = sumOfAdjacentValues / 4.0;
//                    nextMatrix[row][column] = newValue;
//                    delta = absoluteValue(current_matrix[row][column] - newValue);
//                    if (delta > precision) {
//                        terminate = false;
//                    }
//                }
//            }
//        }
//        printMatrix(nextMatrix, rows, columns);
//        copyMatrix(nextMatrix, current_matrix, rows, columns);
//    }
//
//    freeMatrixMemory(nextMatrix, rows);
//    return current_matrix;
//}
//
//void copyMatrix(double **original, double **destination, int rows, int columns) {
//    for (int row = 0; row < rows; ++row) {
//        for (int column = 0; column < columns; ++column) {
//            destination[row][column] = original[row][column];
//        }
//    }
//}
//
//double getAverageOfAdjacentValues(double **current_matrix, int row, int column) {
//    return current_matrix[row][column + 1] + current_matrix[row + 1][column] + current_matrix[row][column - 1] + current_matrix[row - 1][column];
//}
//
//double absoluteValue(double value) {
//    if (value < 0) {
//        return -value;
//    } else {
//        return value;
//    }
//}
//
//bool isEdgeCoordinate(int row, int column, int rows, int columns) {
//    if ((row == 0) || // First Row
//        (row == rows - 1) || // Last Row
//        (column % columns == 0) || // First Column
//        (column % columns == columns - 1)) // Last Column
//    {
//        return true;
//    } else {
//        return false;
//    };
//}
//
//void printMatrix(double **current_matrix, int numberOfRows, int numberOfColumns) {
//    int row, columns;
//
//    for (row = 0; row < numberOfRows; row++) {
//        for (columns = 0; columns < numberOfColumns; columns++) {
//            printf("%f     ", current_matrix[row][columns]);
//        }
//        printf("\n");
//    }
//    printf("\n");
//}