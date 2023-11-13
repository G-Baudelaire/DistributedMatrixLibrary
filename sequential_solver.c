////
//// Created by Germain Jones on 11/11/2023.
////
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//
//void printMatrix(double **matrix, int numberOfRows, int numberOfColumns);
//
//double absoluteValue(double value);
//
//double **allocateMatrixMemory(int rows, int columns);
//
//void freeMatrixMemory(double **matrix, int rows);
//
//bool isEdgeCoordinate(int row, int column, int rows, int columns);
//
//void copyMatrix(double **original, double **destination, int rows, int columns);
//
//double getSumOfAdjacentValues(double **matrix, int row, int column);
//
//double **sequentialSolver(double **matrix, int rows, int columns, double precision);
//
//
//double **allocateMatrixMemory(int rows, int columns) {
//    double **matrix = (double **) malloc(rows * sizeof(double *));
//    double mat[][4] = {{1.0, 1.0, 1.0, 1.0},
//                       {1.0, 0.0, 0.0, 0.0},
//                       {1.0, 0.0, 0.0, 0.0},
//                       {1.0, 0.0, 0.0, 0.0},};
//    if (matrix == NULL) {
//        fprintf(stderr, "Memory allocation failed for rows\n");
//        exit(1);
//    }
//
//    // Allocate memory for each row
//    for (int i = 0; i < rows; i++) {
//        matrix[i] = (double *) malloc(columns * sizeof(double));
//        if (matrix[i] == NULL) {
//            fprintf(stderr, "Memory allocation failed for columns\n");
//            exit(2);
//        }
//    }
//
//    // Initialize with some value
//    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < columns; j++) {
//            if (i == 0 || j == 0){
//                matrix[i][j] = 1.0;
//            } else {
//                matrix[i][j] = 0.0;
//            }
//        }
//    }
//    return matrix;
//}
//
//void freeMatrixMemory(double **matrix, int rows) {
//    for (int i = 0; i < rows; i++) {
//        free(matrix[i]);
//    }
//    free(matrix);
//}
//
//double **sequentialSolver(double **matrix, int rows, int columns, double precision) {
//    bool terminate = false;
//    double sumOfAdjacentValues, newValue, delta;
//    double **currentMatrix = allocateMatrixMemory(rows, columns);
//    double **nextMatrix = allocateMatrixMemory(rows, columns);
//
//    copyMatrix(matrix, currentMatrix, rows, columns);
//
//    while (!terminate) {
//        terminate = true;
//
//        for (int row = 0; row < rows; ++row) {
//            for (int column = 0; column < columns; ++column) {
//                if (!isEdgeCoordinate(row, column, rows, columns)) {
//                    sumOfAdjacentValues = getSumOfAdjacentValues(currentMatrix, row, column);
//                    newValue = sumOfAdjacentValues / 4.0;
//                    nextMatrix[row][column] = newValue;
//                    delta = absoluteValue(currentMatrix[row][column] - newValue);
//                    if (delta > precision) {
//                        terminate = false;
//                    }
//                }
//            }
//        }
//        printMatrix(nextMatrix, rows, columns);
//        copyMatrix(nextMatrix, currentMatrix, rows, columns);
//    }
//
//    freeMatrixMemory(nextMatrix, rows);
//    return currentMatrix;
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
//double getSumOfAdjacentValues(double **matrix, int row, int column) {
//    return matrix[row][column + 1] + matrix[row + 1][column] + matrix[row][column - 1] + matrix[row - 1][column];
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
//void printMatrix(double **matrix, int numberOfRows, int numberOfColumns) {
//    int row, columns;
//
//    for (row = 0; row < numberOfRows; row++) {
//        for (columns = 0; columns < numberOfColumns; columns++) {
//            printf("%f     ", matrix[row][columns]);
//        }
//        printf("\n");
//    }
//    printf("\n");
//}